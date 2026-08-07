# Refactor Plan: Resolving All Code Smells in SE102.N21

**Project:** NESContra (DirectX 11 C++20 Contra remake)
**Source:** all 268 files under `src/`
**Companion:** `code_smell_report.md` (29 smell sections + a 14-item priority list)

This plan resolves **every** smell in §1–§29 and §"Recommendation Priority".
It is written to be executed incrementally: **each phase leaves the project
compiling and the game playable**, verified by `.\build.ps1 -Config Debug -Platform x64`
after every phase. The phases are ordered by **risk first → leverage**, not by the
report's own numbering, so the highest-payoff, lowest-regret changes come first.

> **Method note.** All 29 sections were re-read against the source before
> writing this. Every example citation in the report was verified to match the
> actual code (e.g. `Bill.cpp:347-404` is indeed the 11-branch `dynamic_cast`
> `Fire`; `BulletRState::Enter` does play `"shootM"`; `BillJumpState::Update`
> does use two overlapping `if`s; `Sound::cleanUp` does `delete this`). One
> genuine logic bug was found during verification (see §0, item 10).

## 0. Establish the verification baseline

1. Run `.\build.ps1 -Config Debug -Platform x64`. Confirm **BUILD SUCCEEDED** and
   that `x64\Debug\NESContra.exe` is produced. (VS 2022 Community is installed at
   `C:\Program Files\Microsoft Visual Studio\2022`, so `build.ps1` resolves MSBuild.)
2. Record the baseline: number of warnings, build time, and a 30-second gameplay
   sanity check (move/shoot/jump, die, transition scenes). This is the reference
   every phase is measured against — **a refactor that slows the build or breaks
   gameplay without removing a smell is rejected.**

---

## Phase 1 — Hygiene & mechanical cleanup (low risk, high signal)

*De-risk: these are mechanical, scriptable, and catch regressions early. Touch
~25 files but change <5% of logic. Do them first so later phases aren't
cluttered by noise.*

Maps to: **§2 (stubs), §3 (dead code), §11, §18, §20, §21, §22, §24 (typos/IDs), §29 (build).**

| # | Smell | Action | Files / scope |
|---|-------|--------|---------------|
| 1 | §18 `NULL` everywhere | Replace all `NULL` with `nullptr` project-wide. Scriptable with a single PowerShell/`sed` pass over `src\**\*.{cpp,h}`. | ~15 files (Sound, Soldier, Bill, QuadTreeNode, …) |
| 2 | §18 C-style casts | Replace `(char*)`, `(FLOAT)`, `(HANDLE)`, `(INT)` with `static_cast`/`reinterpret_cast`. | Sound.cpp:81, main.cpp:64, Entity/Bill headers |
| 3 | §11 Redundant alias | Delete `using DEFAULT_TIME = DWORD;`, keep `TIME`. Update the 1 use in `Common.h` (`ANIMATION` tuple). | `src/Common.h` |
| 4 | §20 `pow(x,2.0f)` & no-op | `t*t` instead of `pow(t,2.0f)`; delete `pow(ω,1.0f)` (identity). 3 call sites in `Motion.cpp`. | `Motion.cpp` |
| 5 | §12 Overlapping conditionals | `BillJumpState::Update`: the two `if (vy>=0)` / `if (vy<=0)` blocks both fire at `vy==0`. Convert to `if/else if` so only the rising branch runs at exactly 0. | `BillState/BillJumpState.cpp` |
| 6 | §21 Sprite-rect duplication | `Bill.cpp:131-134` (`BEGIN_01..04`) reuse `JUMP`'s exact rects. Audit and fix to the real begin frames. | `Bill.cpp` LoadSprites |
| 7 | §21 Unicode identifiers | `ω, θ, dω, xO, yO` (Bullet.h/cpp, BulletFState, BulletScubaSoldierState) and `φ` (Motion). Rename to ASCII (`omega`, `theta`, `deltaOmega`, …) for grep-ability. | Bullet headers/cpp, Motion.h/cpp |
| 8 | §3 Commented debug code | Delete the `FOR DEBUG PURPOSE` block in `BillNormalState.cpp:80-105`, the `//int i = 1;` / RSHIFT cycler in `Bill.cpp:100-114`, and the `//bill.SetVX(0.0f)` lines in `Bill.cpp:17-21`. | `BillNormalState.cpp`, `Bill.cpp` |
| 9 | §22 Experimental `Remove` | Delete `QuadTreeNode::Remove` (the `THIS METHOD IS STILL UNDER EXPERIMENTS` stub). It is unfinished and unused; grep confirms no caller. | `QuadTreeNode.cpp/.h` |
| 10 | **New — precedence clarity bug** (found in verification) | `Bill.cpp:384`: `position.x + w / 2.0f * 0.6f` parses as `position.x + (w/2.0f)*0.6f` = `position.x + w*0.3f`. The straight-up/shot states use this; the run/shot states use `position.x + w/2.0f` (true centre). The intent is "spawn at centre-x". Fix to `(position.x + w/2.0f)` and add a comment, **or** if forward-offset is intended make it explicit `position.x + w/2.0f + SHOT_FORWARD_OFFSET`. | `Bill.cpp` Fire |
| 11 | §24 `InsertTexure` typo | Rename to `InsertTexture` everywhere; keep a deprecated inline forwarding alias for any external caller to avoid a hard break. | `GraphicsHelper.{h,cpp}`, all call sites (~10) |
| 12 | §26 Misspelled identifiers | `moveAroundAccelemetor` → `moveAroundAccelerator`; `Unknow` → `Unknown`. | `BossStage3Joint.{h,cpp}` and callers |
| 13 | §26 Mixed-language comments | Convert the Vietnamese comments to English (or move to a `docs/` design note). | `BossStage3Joint.cpp` |
| 14 | §29 `build.ps1` error filter | Change `$_ -match "error"` to `$_ -match 'error[0-9]'` (and `warning[0-9]`) so "0 errors" / "Microsoft" false-positive. Keep the hard-coded VS path as a **fallback only** — make `vswhere` the primary (already is; just don't add more hard-coded paths). | `build.ps1` |

**Verification:** `./build.ps1` succeeds, warning count unchanged or lower. No gameplay change.

---

## Phase 2 — Slay the `dynamic_cast` type-code plague  ← HIGHEST LEVERAGE

*De-risk: this is the report's own #1 item and ~60 of the 109 `dynamic_cast` sites live in one of three fat functions. The fixes are localised to a handful of files but touch the core dispatch architecture, so they are done together and rebuilt before anything else.*

Maps to: **§13 (Bill::Fire, HasWeapons::Fire), §23 (Bullet collision chain), §6 (Stage.cpp explosion switch), §28 (Soldier fire offset ternary, Stage2 string keys).**

### 2.1 `Bill::Fire` — make each state declare its own bullet spawn
- Introduce a small value type in `BillState.h`:
  ```cpp
  struct BulletSpawnParams {
      FLOAT x, y, angle, vx, vy, ax, ay; DIRECTION dir;
  };
  ```
- Add to `State<BillState, Bill>` a **virtual** hook with a default:
  ```cpp
  virtual BulletSpawnParams BillState::BulletSpawnParams(Bill& bill) const;
  ```
  - Default implementation returns "straight ahead from centre" so most states need nothing.
  - Each firing state (`BillJumpState`, `BillNormalShotState`, `BillLayDownState`, both angle-up/shot-straight states, swim states, run-shot) overrides `BulletSpawnParams` and returns **its own** offsets/velocities, computed from `bill.GetX()/GetW()/GetH()/GetMovingDirection()`.
- `Bill::Fire` collapses to:
  ```cpp
  void Bill::Fire() { HasWeapons::Fire(state->BulletSpawnParams(*this)); }
  ```
- This **deletes the 11-branch `dynamic_cast` chain** (lines 347-404) and the
  `position.x + w/2.0f * 0.6f` precedence bug in the same stroke.

### 2.2 `HasWeapons::Fire` — polymorphic bullet creation (kills §13 §14)
- Add two virtuals to `BulletState`:
  ```cpp
  virtual BulletState* Clone() const;      // default: new BulletRState()
  virtual bool IsEnemy() const { return false; }   // default player bullet
  virtual bool IsFake()  const { return false; }
  ```
- Each concrete `BulletState` overrides `Clone()` (cheap — they're trivial to construct) and `IsEnemy()/IsFake()` where non-default.
- `HasWeapons::Fire(BulletSpawnParams)` becomes: build one `Bullet`, set its position/vx/vy/… from the params, set `isEnemy`/`isFake` from the **current state's** `IsEnemy()`/`IsFake()`, `SetState(bulletState->Clone())`, and special-case only the two *genuinely different* spread behaviours (BulletSState 3-round, BulletScubaSoldier 3-round, BulletBossStage2 2-round) by overriding a `virtual std::vector<BulletSpawnParams> SpreadFire(...)`.
- This removes the 8-branch `dynamic_cast<...>` chain in `HasWeapons::Fire` (lines 36-144) **and** the 8-parameter positional `Fire(x,y,angle,vx,vy,ax,ay,dir)` signature (§14) in one move — the new `Fire` takes a single `BulletSpawnParams`.

### 2.3 `Stage::Update` explosion selection — virtual per-enemy (§6, §23)
- Add to `Enemy<T>` a virtual `ExplosionFactory`:
  ```cpp
  virtual Explosion* MakeExplosion() const;
  virtual const char* ExplosionSound() const;
  ```
  - `Enemy` base default → `ExplosionType1`/`"qexplode"` (HUMAN).
  - `Soldier` / `ScubaSoldier` → HUMAN (override not needed).
  - `AirCraft` / `WallTurret` / `Cannon` → MACHINE (`ExplosionType2`/`"exbullet"`).
  - `GunBossStage1` / `FinalBossStage1` / `BossStage3Head` → BOSS (`ExplosionType3`).
  - The drown variant (water death) stays a check on `isDrown` in the single dispatch site.
- `Stage.cpp:104-134` becomes `explosion = enemy->MakeExplosion()` + play `enemy->ExplosionSound()`. The `dynamic_cast<Enemy<Bill>*>` is gone; the `switch` is gone.
- Also fold the **bullet-death** explosion (lines 145-171) into `Bullet::OnDeathExplosion()` so the dead-entity loop is just `entity->OnDeathExplosion(effectEntities)`.

### 2.4 `Bullet::DynamicResolveOnCollision` — virtual collision response (§23)
This is the largest chain (11 `if (dynamic_cast<…>)` blocks, lines 188-288). Two strategies given; pick by risk appetite:

- **Preferred (proper):** add `virtual CollisionResponse Bullet::OnCollisionWith(Entity* other, AABBSweepResult)` and a symmetric `virtual void Entity::OnHitByBullet(Bullet*, AABBSweepResult)` overridden per concrete type (`RockFly`, `Bridge`, `Fire`, `GunBossStage1`, `FinalBossStage1`, `Bill`, `Soldier`, `BossStage3Head/Gate/Hand/Joint`, `TerrainBlock`). `Bullet::DynamicResolveOnCollision` becomes a **single** dispatch: `aabbSweepResult.surfaceEntity->OnHitByBullet(this, aabbSweepResult)`.
- **Lower-risk interim:** keep the chain but replace each `dynamic_cast` with a `static_cast` guarded by a `type_index`/`TypeInfo` enum on `Entity` (one `switch`), which at least removes RTTI cost and centralises the dispatch. Upgrade to virtual later.
- The two near-identical ScubaSoldier/BossStage1 terrain blocks (§23 item 2) collapse into a single `BulletTerrainHitHandler(TERRAIN_BLOCK_TYPE)` helper.
- `isFake`/`isEnemy` flags (§23 item 3): `isEnemy` becomes the virtual `IsEnemy()` from 2.2; `isFake` is only set on `BulletScubaSoldierState` so it can live on that state class instead of on `Bullet` (one field, no flag).

### 2.5 Cross-cutting type-code in expression form (§28)
- `Soldier::Fire` line 32 ternary `dynamic_cast<SoldierShootState*> ? 0.75f : …` → a virtual `FLOAT SoldierState::GunMountOffsetX()` returning the offset. `Soldier::Fire` calls `state->GunMountOffsetX()`.
- `Stage2::TranslateWalls` string `if (name=="L")/("R")/("B")/("T")` → give walls an `enum class WallEdge { Left, Right, Bottom, Top }` and a `Wall::AlignToCamera(Camera*)` virtual, or a `std::unordered_map<std::string, WallEdge>`. Also fix the **asymmetric "B" branch** that uses `camera->GetX()` (centre) while L/R use `GetL()`/`GetR()` — it should mirror L/R semantics; verify against intended letterbox logic.

**Verification:** `./build.ps1` succeeds; the report's #1 item is materially reduced (dynamic_cast count drops from 109 to the double-dispatch sites, ~10–15); gameplay identical except cleaner spawn offsets.

---

## Phase 3 — Collapse the speculative state boilerplate

Maps to: **§2 (132 state classes, 43 `return NULL`), §19 (pragma regions / include coupling for states).**

- **Default no-op on the base `State<BS, DS, E>`** (in `State.h`): give `Exit`/`Enter`/`Render` empty default bodies and make `Update`/`HandleInput` default to `return nullptr`. Concrete states then override **only** what they change. This deletes the `return NULL;` from ~43 `.cpp` files and the empty `Exit`/`Exit`/`HandleInput` stubs from most of the 132. (States that genuinely differ still override — the boilerplate cut is the win, not class deletion.)
- **Collapse near-identical bullet states** (§2 item 3): `BulletR/M/S/LState` are byte-identical except the sound name (`"shootM"`/`"shootS"/…`) and the `BULLET_ANIMATION_ID` passed to `Render`. Replace with **one** `BulletDirectionalState` parameterised by a `SoundName` + `AnimationId` (or a tiny struct). Delete the 4 `.cpp` files and their headers; update the single construction site (`HasWeapons`, the item-switch in `Bill.cpp`).
  - ⚠️ **Fixes §10 in the same move**: the copy-paste bug where `BulletRState` plays `"shootM"` is eliminated because there's now a single constructor that takes the correct name.
- **Scene/Stage states that are pure no-ops:** scan the 132 for states whose `Enter/Render/Update/HandleInput` are all empty and whose `HandleInput` returns `nullptr` — these can share a `NoOpState` base. (Do this as a targeted sweep, not a blanket delete, since some are "dormant until activated".)

**Verification:** build clean; dynamic-cast count on the Fire path is now 0; fewer translation units.

---

## Phase 4 — Data-driven resources, magic numbers & coupling

Maps to: **§1 (God objects / GraphicsHelper + sprite tables), §4 (magic numbers), §5 (data clumps), §9 (feature envy), §14 (param objects), §19 (include coupling), §26 (BossStage3Joint flags).**

### 4.1 Magic numbers → named constants
- `BillJumpState` velocities (`-2.0f`, `+4.00f`, `-0.10f`, `0.05f`) → `BillPhysics::JUMP_SPEED_X`, `JUMP_SPEED_Y`, `GRAVITY`, `DT_60HZ`.
- `GraphicsHelper` capacities (`16384`, `4096`, `1<<22`) → `constexpr` in `GraphicsHelper.h`.
- `Stage.cpp:139,156-168` / `Scene.cpp` offsets (`20.0f`, `3.0f`, `256x240`) → constants.
- `Stage2::CheckIfHasDone` `<= -3.0f` → `BOSS_PASS_JUMP_THRESHOLD`.
- Put all such tunables in a new `src/Constants.h` so designers can tweak without spelunking code.

### 4.2 Data-driven sprite/texture/animation tables (§1, §5, §24)
- The 52 `ENDING_FRAME_NN` entries in `Scene.cpp` (§5 item 2) and the per-class `LoadSprites/Textures/Animations` overloads are hand-written tables — perfect for data driving. The project already depends on `third_party/tileson` (JSON reader). Migrate the **static, repetitive** tables (especially the ending frames: `0,0,256,240` ×52) to a JSON manifest loaded once, generating the `InsertSprite` calls. Keep the *per-class* `Load*` methods but have them read from the manifest instead of inline literals.
- This also lets **§24**'s `GraphicsDatabase` use `at()` (or a checked `GetSprite(id)`) so a missing ID fails loudly instead of silently default-inserting a bogus variant entry (§29 item 3).

### 4.3 Parameter objects (§14)
- The `BulletSpawnParams` from Phase 2.2 already replaces `HasWeapons::Fire`'s 8 args.
- `GraphicsHelper::InsertSprite(SpriteID, x, y, w, h, dir, TextureID)` — bundle into a `SpriteDesc` struct passed by value; `InsertAnimation` already takes a vector, make its first arg a typed `DEFAULT_TIME`. This is cosmetic so defer if time-bound.

### 4.4 Feature-envy cleanup (§9)
- States call `GetVX/SetVX/GetVY/SetVY/SetX/SetY/SetAngle`. Add intent-revealing helpers on `Bill`/`Bullet`/`Entity`: `SetFacingVelocity(FLOAT)`, `ApplyImpulse(FLOAT ax, FLOAT ay)`, `Face(DIRECTION)`. Migrate the states one class at a time; this is ongoing debt-reduction, not a phase gate.

### 4.5 BossStage3Joint 13 booleans → state enum (§26)
- Replace `isMoveAround / isMoveTo / isMoveBy / isMoveByVelocity / isMoveFollow / allowMoveReverse / isMoveAroundAcclemetor / isJointFollowStoped / isMoveFollowFirstTime / isMoveFollowWithAccelemetor` + the implicit ordering with a single:
  ```cpp
  enum class MoveMode { Idle, MoveAround, MoveTo, MoveBy, MoveByVelocity, MoveFollow };
  MoveMode moveMode = MoveMode::Idle;
  ```
  and a `switch (moveMode)` in `Update()`. The "several modes run in one frame" hazard disappears. The `moveFollowWithAccelerator` flag becomes a constructor param of `MoveFollow`. Magic numbers (`16`, `2.5f`, `0.7f`, `11.0f`, `2π`) get named constants here too.
- The empty `LoadSprites/Textures/Animations` stubs (lines 86-99) and the commented `HandleInput` (83) are deleted once the joint's rendering is confirmed to come from its parent hand.

### 4.6 Include decoupling (§19)
- `Bill.cpp` includes 14 headers; `Bullet.cpp` includes 12, and Bill↔Bullet **mutually** include each other. The state forward-declarations in `Bill.h`/`Bullet.h` already exist but the `.cpp` includes are the problem.
- Introduce a `BulletStateFactory` (Phase 2.2's `Clone`) so neither `Bill.cpp` nor `Bullet.cpp` needs the other's full header — they only need the forward declaration + the virtuals. This lets `HasWeapons` own bullet-creation and removes the circular include.
- Strip the 14/12 include lists to just what each TU uses (build with `/showIncludes` to audit).

**Verification:** build clean; warning count should fall (fewer includes → fewer macro collisions).

---

## Phase 5 — Ownership, singletons, globals, RAII (§7, §15, §16, §17, §24, §25, §27)

Maps to: resource-lifetime and global-state smells.

| # | Smell | Action | Files |
|---|-------|--------|-------|
| 1 | §16 `Bullet::SetState` dead null | `Bullet::SetState(BulletState* newState)` sets `newState = NULL;` on a by-value param — a no-op. Remove the line. | `Bullet.cpp:67` |
| 2 | §16 `HasWeapons(BulletState*)` by-value null | `bulletState = NULL;` on the by-value ctor arg is a no-op. Remove. (Ownership is correctly taken by `this->bulletState = bulletState;`.) | `HasWeapons.cpp:13` |
| 3 | §15 Duplicated dead-state guard | The `if (dynamic_cast<BillDeadState*>(state)) { surfaceEntity=NULL; isAbSurface=0; return; }` block is copy-pasted 3× in `Bill` and again in `Soldier`. Extract to `CollidableEntity::BailIfDying()` / `inline bool IsInDeadState()` and call it once at the top of each resolve method. After Phase 2 this is a 1-line check, not an RTTI cast. | `Bill.cpp`, `Soldier.cpp`, `CollidableEntity.h` |
| 4 | §15 `Bullet::Render`/`Bill::Render` transition block | The `if (updateState){ ChangeState(...); updateState=NULL; } if (handleInputState){…}` block is identical in `Bill::Render`, `Bullet::Render`, and `Scene::Render`. Move it into the `State`/`Entity` machinery (a `CommitPendingTransitions()` helper) and reduce the three copies to one call. | `Bill.cpp`, `Bullet.cpp`, `Scene.cpp` |
| 5 | §17 `Sound` self-deleting singleton | `cleanUp()` does `delete this;` while `getInstance()` can still dangle (and `mute()` calls `getInstance()->stop()` after). Rewrite as: `getInstance()` returns a reference; `Shutdown()` does `delete instance; instance = nullptr;`. `main.cpp` calls `Sound::Shutdown()` last. This is the classic leak-free singleton. | `Sound.{h,cpp}`, `main.cpp` |
| 6 | §17 `Sound` raw COM pointers | `IDirectSound8*` and `IDirectSoundBuffer8*` are `Release()`-d by hand. Wrap in `Microsoft::WRL::ComPtr` (already a v143 dependency) and delete the manual `Release()` block in the destructor. | `Sound.h/.cpp` |
| 7 | §17 `loadSound` format-check fallthrough | The `if (bad format) { cout << "…"; }` block (Sound.cpp:103-107) has **no return** — it falls through to use garbage header fields. Add a `return;` there. Also switch `(char*)sfileName` C-style cast to a proper `std::string` and use `<filesystem>` for the path. (§24 flags inconsistent string types; the rest of the code uses `LPCWSTR`/`wstring`, but `Sound` is self-contained so local consistency is acceptable — just make it correct.) | `Sound.cpp:79-107` |
| 8 | §27 `GraphicsHelper` raw device pointers | `device`/`context`/`swapChain` are exposed as raw static `ID3D11Device*` etc. and `Release()`-d by hand in `Cleanup()` (lines 630-693). Convert to module-static `ComPtr<ID3D11Device>` etc.; `Cleanup` just `.Reset()`s them (ComPtr handles `Release`). | `GraphicsHelper.{h,cpp}` |
| 9 | §27 `GraphicsDatabase` raw texture pointers | `ID3D11ShaderResourceView*` stored raw and `new`'d `RECT*` released by hand. Use `ComPtr` for SRVs and `std::unique_ptr<RECT>` (or store `RECT` by value) for sprite rects. | `GraphicsDatabase.h`, `GraphicsHelper.cpp` |
| 10 | §25 `HasWeapons::bullets` global list | `static std::list<Bullet*> bullets` shared by every weapon, drained by `Stage::Update`. Move bullet storage onto `Stage` (already has `effectEntities`) and pass a reference to weapons, or have `Scene` own a `BulletManager`. | `HasWeapons.{h,cpp}`, `Stage.{h,cpp}` |
| 11 | §7 / §25 module globals in `main.cpp` | `scene`/`input`/`camera` are file-scope. Move all three inside `WinMain` as locals (they're only used there) and thread references to the parts that need them. | `main.cpp` |
| 12 | §24 `Bool<T>` lazy-flag abstraction | `template<class T> struct Bool { BOOL value; }` exists *only* so each `HasSprites<T>`/`HasTextures<T>`/`HasAnimations<T>` gets a distinct static flag — a 2-line hack around not knowing about `inline` variables. Replace with `inline static bool hasLoaded = false;` inside the `Has*` templates (C++17 `inline` variables; the project is C++20). Removes the `Bool<T>` struct from `Common.h` and the `.value` member access across ~15 files. | `Common.h`, `HasSprites/Textures/Animations.h` |
| 13 | §29 `GraphicsDatabase` `operator[]` silent insert | Replace `GraphicsDatabase::animations[id]`/`sprites[id]` with a checked `At(map, id)` that asserts in debug / returns `nullptr` (or logs) on miss, so a missing ID is a visible error, not a default-inserted bogus entry. | `GraphicsDatabase.h` usage sites |
| 14 | §16 QuadTreeNode raw new / comma-operator | `QuadTreeNode::Insert`/`Clear` use `new`/`Destroy` with `this->nodes[i]->Clear(), Destroy(this->nodes[i]);`. Convert the 4 children to `std::unique_ptr<QuadTreeNode> nodes[4]` and delete the manual `Destroy` calls. Also replaces the magic `i <= 3` (§22) with a named `CHILD_COUNT` or range-for. | `QuadTreeNode.h/.cpp` |

**Verification:** build clean; run the game and confirm death/restart/explosion/weapon-swap still behave, and that `Sound` no longer dangles after shutdown.

---

## Phase 6 — Architectural paydown (§8, §29 shim, and the rest)

Maps to: structural smells that are correct to fix but **large and cross-cutting**.

### 6.1 Open/Closed violation (§8)
`src/Common.h:63` comment: *"If u add an object, u must add id of object here."* Every new object appends to three `std::variant` lists and a forward-declaration block in `Bill.h`.
- **Target:** a single source of truth. Generate the `SPRITE_ID`/`TEXTURE_ID`/`ANIMATION_ID` variants (and the `*_COMMON.h` enum files) from one JSON registry (`Resources/registry.json` or `src/ObjectRegistry.json`) via a small code-gen script (`tools/GenIds.py`) run as a pre-build step. Adding an object then means editing one JSON file.
- **Lower-effort interim:** keep the variants but add a `static_assert`/CI check that flags when a new `*_SPRITE_ID` enum exists but isn't in the variant list, so the "must remember" step is enforced, not manual.
- This is the biggest structural debt; treat it as a **tracked epic**, not a single PR.

### 6.2 The `DX11Math.h` shim (§29)
`DX11Math.h` (188 lines) reinvents `D3DXVECTOR2/3`, `D3DXMATRIX`, color macros and vector helpers on top of DirectXMath, then `reinterpret_cast`s between its matrices and `XMFLOAT4x4` (latent UB). It exists "so game-logic files compile without modifications."
- **Path 1 (big bang, risky):** replace all `D3DXVECTOR*` usages with `DirectX::XMFLOAT2/3` and delete the shim. ~25 files touched, high merge cost.
- **Path 2 (incremental, recommended):** leave the shim's *type aliases* (they're harmless `static_assert`-sized wrappers) but stop *re-implementing* math — route every vector op through DirectXMath's inline helpers (`XMVectorAdd`, `XMVectorScale`, etc.) and add `static_assert(sizeof(D3DXVECTOR3)==sizeof(XMFLOAT3))` so the layout bet is *checked*, not assumed. Migrate the shim off `reinterpret_cast` to `memcpy`/explicit getters.
- Defer a full removal until the game ships; the shim is *working* technical debt, not broken debt.

### 6.3 HasAnimations god-template (§29)
`HasAnimations::SetAnimation` (30 inlined lines that index `GraphicsDatabase`, mutate frame state, and call `DrawSprite`) is a god template. Once Phase 2's data-driven loaders exist, split it into: `AdvanceFrame()`, `ResolveCurrentSprite()`, `Draw()`. Low priority — it works.

### 6.4 `#pragma region` markers (§19)
47 region markers exist only to fold over-sized `LoadSprites/Textures/Animations` tables. Once those are data-driven (4.2), the regions vanish with no replacement needed.

---

## Dependency & ordering map

```
Phase 1 (hygiene)      → builds alone, no deps
Phase 2 (type-code)    → depends on Phase 1 (nullptr cleanliness); unlocks Phase 3 bullet-collapse & HasWeapons rewrite
Phase 3 (state/param)  → depends on Phase 2 (Fire/BulletSpawnParams exists)
Phase 4 (data/Coupling)→ runs in parallel with 3; BossStage3Joint enum is independent
Phase 5 (RAII/globals) → independent; do alongside 4
Phase 6 (architecture) → long-running epics; start §8 code-gen and §29 shim hardening after 1–5
```

## Effort & risk register

| Risk | Phases | Why | Mitigation |
|------|--------|-----|------------|
| Gameplay change from rewritten dispatch | 2, 3 | Bullet spawn offsets / explosion types must match pixel-for-pixel | Diff the computed `BulletSpawnParams` against the old literals table; run the same 30s smoke test. |
| Mass recompile after base-class edits | 2, 3 | `State.h` / `BulletState` signature changes touch all 132 states | Do 2.1 (Bill) and 2.2 (Bullet) as one atomic commit; rebuild once. |
| Self-deleting singleton regression | 5 | `Sound` is used in every state's `Enter` | Verify the audio cue plays on shoot/death/explosion post-refactor. |
| Shim UB from matrix reinterpret_cast | 6 | Latent, not currently crashing | Add `static_assert` first; defer removal. |
| Build-script false-positive errors | 1 | `build.ps1` `error` filter | Fix regex; confirm by reading real error lines. |

## Per-phase verification contract

Every phase ends with:
1. `.\build.ps1 -Config Debug -Platform x64` → **BUILD SUCCEEDED**, warning count ≤ previous.
2. One smoke run: spawn → move/shoot/jump → die → scene transition → restart. No regressions.
3. `git diff --stat` reviewed so no smell-regression (e.g. a new `dynamic_cast`) is reintroduced.

## Mapping: smell → phase (full coverage)

| § | Smell | Phase |
|---|-------|-------|
| §1 | God Object (GraphicsHelper, Bill.h, Scene.cpp tables) | 4.2, 4.5 |
| §2 | Duplicate/speculative state classes, `return NULL` | 3 |
| §3 | Dead commented code | 1 (item 8) |
| §4 | Magic numbers | 4.1 |
| §5 | Data clumps / primitive obsession | 4.2, 4.3 |
| §6 | Switch-on-type → polymorphism (Stage.cpp) | 2.3 |
| §7 | Singleton + hidden globals (Sound, main.cpp, GraphicsHelper) | 5 (items 5, 11, 8) |
| §8 | Open/Closed (variant lists) | 6.1 |
| §9 | Feature envy (states reach into host) | 4.4 |
| §10 | Copy-paste bug (BulletR → shootM) | 3 (bullet collapse) |
| §11 | Redundant type aliases (TIME/DEFAULT_TIME) | 1 (item 3) |
| §12 | Overlapping conditionals (BillJumpState) | 1 (item 5) |
| §13 | dynamic_cast type-code (Bill::Fire, HasWeapons::Fire) | 2.1, 2.2 |
| §14 | Long parameter lists (HasWeapons::Fire, InsertSprite) | 2.2, 4.3 |
| §15 | Duplicated guard logic / transition block | 5 (items 3, 4) |
| §16 | Dead ownership code (SetState, by-value nulling, QuadTreeNode) | 1.10, 5 (items 1, 2, 14) |
| §17 | Sound singleton self-delete + raw COM | 5 (items 5, 6, 7) |
| §18 | NULL vs nullptr / C-style casts | 1 (items 1, 2) |
| §19 | Include-everything / pragma regions | 4.5, 6.4 |
| §20 | Near-identical Motion helpers + pow misuse | 1 (item 4) |
| §21 | Data duplication + non-portable identifiers | 1 (items 6, 7) |
| §22 | Incomplete Experimental code | 1 (item 9) |
| §23 | Bullet collision type-code chain + isFake/isEnemy flags | 2.2, 2.4 |
| §24 | Suspicious abstractions & naming (Bool<T>, InsertTexure, Sound API) | 5 (item 12), 1 (items 11, 7) |
| §25 | Shared mutable state / global collections | 4.2, 5 (items 10, 11) |
| §26 | BossStage3Joint 13-boolean FSM | 4.5 |
| §27 | Inconsistent resource mgmt (GraphicsHelper) | 5 (items 8, 9) |
| §28 | Duplicated guards / Soldier ternary / Stage2 strings | 2.5, 5 (item 3) |
| §29 | DX11Math shim, god-template SetAnimation, operator[] insert, global registry, build.ps1 | 6.2, 6.3, 5 (item 13), 1 (item 14) |

**All 29 sections are covered; the four "Recommendation Priority" items map to Phase 2 (item 1), Phase 1 (item 2), Phase 3 (item 3), and Phase 4 (item 5/6).**
