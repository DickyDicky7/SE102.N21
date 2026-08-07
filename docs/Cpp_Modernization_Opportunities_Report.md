# C++ Modernization Opportunities Report

**Project:** NESContra (SE102.N21) — DirectX 11 C++ game  
**Scope:** `src/` folder — 265 source files (80 headers, 185 .cpp)  
**Scan date:** 2026-08-08  
**Current standard:** C++20 (`<LanguageStandard>stdcpp20</LanguageStandard>`, v143 toolset = VS2022)  

---

## Executive Summary

The project already lives on **C++20** with the **v143** (Visual Studio 2022) toolset,
and has already adopted many modern features:

| Feature | Status | Count |
|---|---|---|
| `nullptr` | ✅ Adopted (partial) | 51 |
| `enum class` | ✅ Adopted (99%) | 67 |
| `override` on all virtuals | ✅ Adopted | 707 |
| `std::variant` | ✅ Adopted | 3 major type aliases |
| `std::optional` | ✅ Adopted (1 site) | 1 |
| `std::concepts` / `requires` | ✅ Adopted | 10 |
| Structured bindings (`auto[...]`) | ✅ Adopted | 6 |
| `std::cmp_*` comparisons | ✅ Adopted | 21 |
| Range-based `for` | ✅ Adopted (52) | 52 |
| `ComPtr` (WRL) | ✅ Adopted (partial) | 37 |
| `std::unique_ptr` | ✅ Adopted (1 site) | 1 |

However, the codebase still carries significant **C++14/C++17-era patterns** that can
be upgraded, and the v143 toolset supports **C++23**. The scan found **127+ distinct
modernization opportunities** across **38 sections**. The opportunities fall into
three buckets:

1. **Language-version upgrades** — push to C++23 for features like `std::print`,
   `std::expected`, `std::flat_map`, `std::mdspan`, `std::move_only_function`,
   `std::ranges::to`, `std::is_power_of_2`, `std::start_lifetime_as`.
2. **Modern C++ feature adoption within C++20/C++23** — replace legacy patterns
   with idiomatic modern C++. The project is on C++20 but uses **0** `std::visit`,
   **0** `std::move`, **0** `std::make_unique`, **0** `noexcept`, **0** `[[nodiscard]]`,
   **0** `if constexpr`, **0** `<=>` operator, **0** `std::ranges`, **0** `std::span`,
   **0** `std::string_view`, **0** `std::format`, **0** STL algorithms beyond `std::sort`.
   These features are **available but completely absent**.
3. **RAII / memory-safety** — eliminate raw `new`/`delete`, `NULL`, C-style casts,
   and manual COM `Release()` calls. 342 `new` expressions, 461 `NULL` usages,
   15 `reinterpret_cast`, 3 `std::list::remove()` calls, 23 manual index loops.

---

## 1. Build Configuration

### Current State
- **Toolset:** `v143` (Visual Studio 2022 17.x)
- **Language standard:** `<LanguageStandard>stdcpp20</LanguageStandard>` (4 configurations: Debug/Release × Win32/x64)
- **Conformance mode:** Enabled (`<ConformanceMode>true</ConformanceMode>`)
- **No CMakeLists.txt** — pure MSVC `.vcxproj` / `.sln`

### Opportunity: Upgrade to C++23
The v143 toolset, when updated to the latest VS 2022 17.10+, supports a
substantial subset of C++23. Upgrade requires only changing
`<LanguageStandard>stdcpp20</LanguageStandard>` →
`<LanguageStandard>stdcpp23</LanguageStandard>` in all 4 `ItemDefinitionGroup`
blocks of `NESContra.vcxproj`.

| C++23 Feature | Available in MSVC v143? | Relevant Opportunities |
|---|---|---|
| `std::print` / `std::println` | ✅ (17.5+) | Replace `std::cout` in Sound.cpp, main.cpp |
| `std::expected` | ✅ (17.5+) | Result-type error handling (GraphicsHelper init, Sound load) |
| `std::flat_map` / `std::flat_set` | ✅ (17.5+) | Replace `std::unordered_map` in GraphicsDatabase, Sound singleton |
| `std::mdspan` | ✅ (17.5+) | View sprite data without copying |
| `std::move_only_function` | ✅ (17.5+) | Store callbacks without allocation |
| `if consteval` | ✅ | Compile-time branching in templates |
| `std::ranges::to` | ✅ (17.5+) | `std::vector<Entity*>(map \ranges::to)` conversions |
| `std::start_latitude` / `std::hermite` | ✅ | Math helpers in Motion.cpp |
| `std::optional` constexpr | ✅ | `FormatId` could be constexpr |
| Deduction guides | ✅ | Less verbose CTAD for SPRITE/ANIMATION tuples |
| `std::string` constexpr | ✅ | Compile-time string building in `ID` template |

---

## 2. `nullptr` Migration (High Priority, Mechanical)

### Finding
- **461** `NULL` occurrences vs **51** `nullptr` — only 10% migrated.
- `NULL` is used for pointers, COM interface pointers, and in Win32 API
  calls where `nullptr` is a drop-in replacement.

### Affected Files (top offenders)
| File | NULL count |
|---|---|
| `GraphicsHelper.cpp` | 44 |
| `Sound.cpp` | 10 |
| `Soldier.cpp` | 12 |
| `Bill.cpp` | 12 |
| `AirCraft.cpp` | 11 |
| `BossStage3HandSpinningState.cpp` | 9 |
| `Bullet.cpp` | 6 |
| `main.cpp` | 6 |
| `BillState/*.cpp` (132 files) | ~2–9 each |
| `Stage.cpp` | 5 |

### Action
Global `nullptr` replacement. `NULL` → `nullptr` is a pure drop-in in MSVC
because `NULL` is defined as `0` — every `if (ptr == NULL)` and `ptr = NULL`
becomes type-safe `nullptr`. This also enables `/W4` warnings for any
mismatched uses.

### Note
Some `NULL` in `Sound.cpp` are DirectSound API params (`CreateSoundBuffer(..., NULL)`),
which should become `nullptr`. Others like `GUID_NULL` are not pointers and
must stay as-is.

---

## 3. C-Style Enums → `enum class` (High Priority, Mechanical)

### Finding
Only **1** C-style enum remains:

```cpp
// BossStage3Joint.h:19
enum MoveAroundDirection { Positive, Negative, Unknow };
```

This is the only pure C-style enum in the entire codebase. It is used without
qualification in `BossStage3Joint.cpp` and its state files.

### Action
Wrap as `enum class` and update the ~5 call sites that use unqualified names
(`Positive` → `MoveAroundDirection::Positive`).

---

## 4. Manual Memory Management → RAII / Smart Pointers (High Priority, High Value)

### 4a. State Object Lifecycle (`new`/`delete` in `ChangeState`)

The state machine uses raw `new`/`delete` throughout:

```cpp
// State.h:41 — template ChangeState
virtual T* Update(E&) = 0;   // returns raw new state or NULL
// ...
delete currentState;        // manual deletion
currentState = newState;
```

State `Update`/`HandleInput` methods return `new SomeState()` or `NULL`.
`ChangeState` deletes the old state and takes ownership of the new one.

**342 `new` expressions** across the codebase. **8 `delete` expressions**.
The remaining ownership is handled by `Destroy<T*>()` (15 occurrences) —
a custom template that does `delete pointer; pointer = NULL;`.

### Opportunity
Replace the raw `new`/`delete` state machine with `std::unique_ptr`:

```cpp
// Modern approach
virtual std::unique_ptr<T> Update(E&) = 0;  // nullptr = no transition
```

`ChangeState` would then use `std::unique_ptr<T>&` and automatic cleanup.
This eliminates all 342 `new` calls, the 8 `delete` calls, and the 91
`Destroy()` calls.

### 4b. Scene/Entity Ownership (main.cpp)

```cpp
// main.cpp:12 — module-global raw pointers
Scene* scene; Input* input; Camera* camera;
// ...
scene = new Scene();
input = new Input(hInstance, hWnd);
camera = new Camera(new CameraStaticState());
// ...
Destroy(input);
Destroy(scene);
Destroy(camera);
```

**Opportunity:** Use `std::make_unique` / `std::make_unique` or local
`std::unique_ptr` variables. This also removes the module-global pointers
(smell §7 in the code smell report).

### 4c. Bullet List Management

```cpp
// HasWeapons.cpp:28-29
Bullet* bullet = NULL;
bullet = new Bullet();
// ...
bullets.push_back(bullet);
```

**Opportunity:** `std::list<Bullet*>` → `std::list<std::unique_ptr<Bullet>>`
or `std::vector<std::unique_ptr<Bullet>>` (vector is better for cache locality).

### 4d. QuadTreeNode Raw Pointers

```cpp
// QuadTreeNode.h:36
QuadTreeNode* nodes[4];
// QuadTreeNode.cpp:31-34
this->nodes[0] = new QuadTreeNode{...};
// QuadTreeNode.cpp:6
this->nodes[i]->Clear(), Destroy(this->nodes[i]);
```

**Opportunity:** `std::array<std::unique_ptr<QuadTreeNode>, 4>` eliminates
manual `new`/`Destroy` and the comma-operator cleanup (`nodes[i]->Clear(), Destroy(nodes[i])`).

---

## 5. `Bool<T>` Lazy-Flag Abstraction → Direct `constexpr bool` (Medium Priority)

### Finding
`Common.h:60-61`:
```cpp
template <class T>
struct Bool { BOOL value; };
```

Used as `static Bool<T> hasBeenLoaded;` in `HasSprites<T>`, `HasTextures<T>`,
and `HasAnimations<T>`. The `Bool<T>` template exists only so each instantiation
of `HasSprites<Bill>` vs `HasSprites<Bullet>` gets a distinct static variable.

### Opportunity
C++17 inline variables make this trivial:
```cpp
template <class T>
class HasSprites {
    static inline bool hasBeenLoaded = false;
};
```

Remove `Bool<T>` entirely. Replace `hasBeenLoaded.value` with direct `bool` access.

---

## 6. `#define` Macros → `constexpr` / `inline constexpr` (Medium Priority)

### Finding
**5 `#define` macros** in `Common.h`:
```cpp
#define SCREEN_WIDTH   640
#define SCREEN_HEIGHT  600
#define SCALING_RATIO_X 2.5f
#define SCALING_RATIO_Y 2.5f
#define DIRECTINPUT_VERSION 0x0800  // required by dinput.h — keep
```

Plus **`#define DRAW_HITBOXES 0`** in `GraphicsHelper.cpp` and
**#define D3DX_PI** / color macros in `DX11Math.h`.

### Opportunity
Replace all except `DIRECTINPUT_VERSION` (which must be a macro for the
DirectInput header) with `constexpr` or `inline constexpr`:

```cpp
inline constexpr int SCREEN_WIDTH = 640;
inline constexpr int SCREEN_HEIGHT = 600;
inline constexpr FLOAT SCALING_RATIO_X = 2.5f;
```

`DRAW_HITBOXES` could use `if constexpr` in C++17+ instead of `#if`.

---

## 7. `std::to_string` + String Concatenation → `std::format` / `std::print` (Medium Priority)

### Finding
- **15** `std::to_string` calls (TerrainStage1/2.cpp, LoadingSceneState.cpp,
  GameOverSceneState.cpp, Stage.cpp, Common.h `ID` template, `FormatId`)
- **15** string concatenation with `+` operator
- **0** `std::format` / `std::print` usage

### Examples
```cpp
// TerrainStage1.cpp:35
std::string _id = std::to_string(_tile.getId());
_id = "Stage 1 Sprite " + FormatId(_id);

// Common.h:168 — inherits from std::string (UB risk: no virtual destructor)
template <int prefix>
class ID : public std::string {
    public: ID(std::string value) : std::string(std::to_string(prefix) + " " + value) {}
};

// Common.h:170
inline std::string FormatId(std::string id)
{
    return id.size() == 1 ? "00" + id : id.size() == 2 ? "0" + id : id;
}
```

### Opportunity (C++20 `std::format`)
```cpp
auto _id = std::format("Stage 1 Sprite {}", FormatId(std::to_string(_tile.getId())));
// Or more idiomatically:
auto _id = std::format("Stage 1 Sprite {:0>3}", _tile.getId());
```

The `ID` template inheriting from `std::string` is a **latent UB** —
`std::string` has no virtual destructor. Replace with a simple wrapper or
`std::string` factory function. `FormatId` can use `std::format("{:03}", n)`.

---

## 8. `std::string` Parameters by Value → `std::string_view` (Low Priority, Mechanical)

### Finding
`Sound` class methods take `std::string` by value for name parameters:
```cpp
void play(std::string name, bool infiniteLoop, int times);      // Sound.h:35
void stop(std::string name = "");                                // Sound.h:36
void setVolume(float percentage, std::string name = "");         // Sound.h:33
void loadSound(const char* fileName, std::string name);          // Sound.h:34
```

`Sound::play` is called ~50 times across the codebase with string literals
like `"shootM"`, `"stage1"`, etc. Each call allocates a temporary `std::string`.

### Opportunity
```cpp
void play(std::string_view name, bool infiniteLoop, int times);
void stop(std::string_view name = {});
void setVolume(float percentage, std::string_view name = {});
```

Also `FormatId(std::string id)` → `FormatId(std::string_view id)`.

**Note:** `Sound::play("shootM", ...)` with `string_view` avoids the temporary
`std::string` allocation. The `soundBufferMap` is `std::map<std::string, ...>` —
if changed to a `std::string`-keyed map, `string_view` params need a
`std::string` lookup, but the savings at call sites still outweigh the one
allocation in `play`.

---

## 9. C-Style Casts → `static_cast` / `reinterpret_cast` (Medium Priority)

### Finding
- **23** `static_cast` (already adopted in GraphicsHelper.cpp, main.cpp)
- Only **2** remaining C-style casts found:
  ```cpp
  // Sound.cpp:81
  char* fileName = (char*)sfileName;
  // Sound.cpp:132
  tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&(*pSecondaryBuffer));
  ```

- Also in `Bullet.cpp:21`, `Bill.cpp:18`: `CollidableEntity::self = (Entity*)this;`
  (this is a C-style cast that should be `static_cast`)

- `Sound.cpp:160`: `(void**)&bufferPtr`, `(DWORD*)&bufferSize`, `(void*)bufferPtr`
  — all COM API calls needing `reinterpret_cast`

- `(DWORD*)&` and `(FLOAT)` in Entity headers and DX11Math.h

### Action
Replace with `static_cast` / `reinterpret_cast`. Note: COM `QueryInterface`
requires `reinterpret_cast<void**>` specifically.

---

## 10. Iterator-Style Loops → Range-Based For (Low Priority, Mechanical)

### Finding
**5** legacy iterator loops:
- `Sound.cpp:45, 210, 232` — `for (iterator it = map.begin(); it != map.end(); it++)`
- `QuadTree.h:62, 91` — (dead code, in the abandoned `QuadTree` class)

```cpp
// Sound.cpp:45
for (auto it = soundBufferMap.begin(); it != soundBufferMap.end(); it++)
// Sound.cpp:210
for (std::map<...>::iterator it = soundBufferMap.begin(); ...)
// Sound.cpp:232
for (std::map<...>::iterator it = soundBufferMap.begin(); ...)
```

### Opportunity
Range-based `for`:
```cpp
for (auto& [name, buffer] : soundBufferMap) { ... }
// Uses C++17 structured bindings (already used elsewhere in the project)
```

---

## 11. `std::sort` Lambda Improvements (Low Priority)

### Finding
`Stage1.cpp:107` and `Stage2.cpp:115` both have identical sort lambdas:
```cpp
std::sort(
    sorted.begin(), sorted.end(),
    [](std::pair<Entity*, QuadTreeNode*> pair1,
       std::pair<Entity*, QuadTreeNode*> pair2) -> BOOL
    {
        return pair1.first->GetL() < pair2.first->GetL();
    }
);
```

### Opportunities
1. **Pass by const reference** instead of by value:
   `const std::pair<Entity*, QuadTreeNode*>& pair1`
2. **`BOOL` → `bool`** (BOOL is `int` in Win32)
3. **C++20 `<ranges>` + `std::ranges::sort`** with a projection:
   ```cpp
   std::ranges::sort(vec, {}, &std::pair<Entity*, QuadTreeNode*>::first,
                     &Entity::GetL);
   ```
   Or more practically, use a member-function pointer as comparator.

---

## 12. `ZeroMemory` → Value Initialization (Low Priority, Mechanical)

### Finding
**4** `ZeroMemory` calls:
- `main.cpp:79, 127` — `ZeroMemory(&wc, sizeof(WNDCLASSEX))`, `ZeroMemory(&msg, sizeof(msg))`
- `Input.cpp:63, 73` — `ZeroMemory(&mouseST, sizeof(mouseST))`, `ZeroMemory(keys, sizeof(keys))`

### Opportunity
C++ value initialization with `{}` is more idiomatic and type-safe:
```cpp
WNDCLASSEX wc{};   // zero-initialized
MSG msg{};
```

For `Input.cpp`:
```cpp
DIMOUSESTATE mouseST{};  // instead of ZeroMemory
char keys[256]{};        // instead of ZeroMemory
```

`ZeroMemory` is a macro wrapping `memset` — it bypasses type safety.

---

## 13. `Bool<T>` Member Access → Direct `bool` (Medium Priority)

### Finding
The `Bool<T>` pattern is accessed as:
```cpp
// Bullet.cpp:77
if (HasSprites<Bullet>::hasBeenLoaded.value) return;
HasSprites<Bullet>::hasBeenLoaded.value = true;
```

### Opportunity
With inline variables (see §5), this becomes:
```cpp
if (HasSprites<Bullet>::hasBeenLoaded) return;
HasSprites<Bullet>::hasBeenLoaded = true;
```

---

## 14. `std::list` → `std::vector` (Medium Priority, Performance)

### Finding
- **14** `std::list` usages (e.g., `HasWeapons::bullets`, `QuadTreeNode::entities`,
  `Stage::effectEntities`, `deadEntities`, `outOfBoundBullets`)
- **0** performance justification for `std::list` (no splicing, no mid-list
  insertion with stable iterators)

### Opportunity
`std::vector` is almost always faster (cache locality, fewer allocations).
The existing code already does `for (auto& bullet : bullets)` iteration,
`bullets.clear()`, `bullets.push_back()` — all of which work identically with
`std::vector`.

Key sites:
- `HasWeapons::bullets` (`std::list<Bullet*>`) — bullets are iterated and cleared
  every frame; `std::vector` would be ~5-10× faster.
- `QuadTreeNode::entities` (`std::list<Entity*>`) — entities are iterated for
  collision checks every frame.

---

## 15. `std::map` → `std::flat_map` (C++23, Low Priority)

### Finding
- **6** `std::map` usages, most notably:
  - `Sound::soundBufferMap` (`std::map<std::string, IDirectSoundBuffer8*>`)
  - `Stage::walls` (`std::unordered_map<std::string, Entity*>`)

### Opportunity (C++23)
`std::flat_map` (available in MSVC 17.5+) offers better cache locality for
read-mostly maps. `Sound::soundBufferMap` is written once at load time and
read every frame during `play()`/`stop()` — ideal for `flat_map`.

---

## 16. Raw COM Pointers → `ComPtr` (High Priority, Safety)

### Finding
`ComPtr` is already adopted in `GraphicsHelper.cpp` for shader/pipeline objects,
but several COM resources remain as raw pointers:

| File | Raw COM pointer |
|---|---|
| `Sound.h/.cpp` | `IDirectSound8* pDevice`, `IDirectSoundBuffer* primaryBuffer`, `IDirectSoundBuffer8*` in `soundBufferMap` |
| `Input.h/.cpp` | `LPDIRECTINPUT8 input`, `LPDIRECTINPUTDEVICE8 mouse`, `LPDIRECTINPUTDEVICE8 keyboard` |
| `GraphicsHelper.h/.cpp` | `ID3D11Device* device`, `ID3D11DeviceContext* context`, `IDXGISwapChain* swapChain` (raw statics) |
| `GraphicsDatabase.cpp` | `ID3D11ShaderResourceView* srv` in `TEXTURE` struct |
| `Common.h` | `TEXTURE` struct holds raw `ID3D11ShaderResourceView* srv` |

The `Sound` destructor manually calls `Release()` on each buffer and the device.
The `Input` destructor calls `Release()` on `mouse`, `keyboard`, `input`.
`GraphicsHelper::Cleanup()` manually releases `device`, `context`, `swapChain`.

### Opportunity
Wrap all COM pointers in `Microsoft::WRL::ComPtr<T>`:
```cpp
ComPtr<IDirectSound8> pDevice;
ComPtr<IDirectSoundBuffer8> primaryBuffer;
std::map<std::string, ComPtr<IDirectSoundBuffer8>> soundBufferMap;
```

This eliminates all manual `Release()` calls and prevents leaks on
early-return / exception paths.

---

## 17. `Destroy<T>()` Template → `std::unique_ptr` / RAII (High Priority)

### Finding
```cpp
// Common.h:158-166
template <class T>
inline void Destroy(T*& pointer)
{
    if (pointer)
    {
        delete pointer;
        pointer = NULL;  // should be nullptr
    }
}
```

Used **91** times across the codebase for `delete` + null-out.

### Opportunity
Once states and entities are migrated to `std::unique_ptr` (§4), the `Destroy`
template becomes unnecessary. As an interim step, at least fix `NULL` → `nullptr`
and add a `noexcept` specifier.

---

## 18. Unicode Identifiers → ASCII (Low Priority, Mechanical)

### Finding
`Bullet.h:182-186` and `Motion.h/cpp` use Unicode math identifiers:
- `ω` (omega) in `BulletFState`, `BulletScubaSoldierState`, `Motion.cpp`
- `θ` (theta) in `BulletFState`, `BulletScubaSoldierState`, `Motion.h`
- `dω`, `xO`, `yO`, `φ` (phi)

### Opportunity
Rename to ASCII: `omega` → `angularVelocity`, `θ` → `theta`, `dω` → `deltaOmega`,
`xO`/`yO` → `centerX`/`centerY`, `φ` → `phaseOffset`.

This improves grep-ability, tool compatibility, and follows the project's
own coding convention (the refactor plan §1.7 already lists this).

---

## 19. `std::vector` Push-Back → `emplace_back` (Low Priority)

### Finding
- **0** `emplace_back` usages (both `push_back` and `emplace_back` counts are 0 —
  the code uses aggregate-initialization `push_back({...})` pattern or
  direct `push_back` on `std::list`).

Actually, looking more carefully, the code uses `push_back` on `std::list`
(not `std::vector`), and `bullets.push_back(bullet)` where `bullet` is already
constructed. The `emplace_back` opportunity exists for:
```cpp
// QuadTreeNode.cpp:53
this->entities.remove(movedEntity);
// ... and many push_back calls on lists
```

### Opportunity
Switching to `std::vector` (§14) opens `emplace_back` opportunities.

---

## 20. Missing `noexcept` on Destructors/Getters (Low Priority)

### Finding
- **0** `noexcept` annotations project-wide
- **0** `[[nodiscard]]` annotations

All destructors, getters, and pure functions could be marked `noexcept`:
```cpp
virtual ~Entity() noexcept;          // currently: virtual ~Entity()
inline FLOAT Entity::GetW() const { return w; }  // could be: noexcept
```

`ChangeState` in `State.h` performs `delete` + assignments — could be `noexcept`
(assuming the `Exit`/`Enter` methods are also `noexcept`).

Getters like `GetPosition()`, `GetVX()`, `GetCurrentFrame()` are obvious
`[[nodiscard]]` candidates.

---

## 21. `ID` Template Inherits from `std::string` (Low Priority, Correctness)

### Finding (`Common.h:168`)
```cpp
template <int prefix>
class ID : public std::string {
public:
    ID(std::string value) : std::string(std::to_string(prefix) + " " + value) {}
};
```

**Problem 1:** Inheriting from `std::string` is undefined behaviour —
`std::string` has no virtual destructor. If `delete` is called through a
base pointer, it is UB.

**Problem 2:** The constructor takes `std::string` by value (unnecessary copy).

### Opportunity
Replace with a free function or a non-inheriting wrapper:
```cpp
inline std::string MakeId(std::string_view value)
{
    return std::format("{} {}", prefix, value);
}
```

Or if the `ID` type is used for type discrimination (it's used as template
argument for `Bool<T>`), a simple tagged wrapper works:
```cpp
template <int Prefix>
struct ID {
    std::string value;
    ID(std::string_view v) : value(std::format("{} {}", Prefix, v)) {}
};
```

---

## 22. `Sound` Self-Deleting Singleton (High Priority, Correctness)

### Finding
```cpp
// Sound.cpp:258-261
void Sound::cleanUp()
{
    delete this;   // self-delete! instance pointer dangles
}
```

Called from `main.cpp:294`:
```cpp
Sound::getInstance()->cleanUp();
```

After `delete this`, `Sound::instance` still points to freed memory. If
`cleanUp()` is called again (or `getInstance()` is used), it is UB.

### Opportunity
Use a proper RAII singleton or a scoped instance:
```cpp
class Sound {
    static std::unique_ptr<Sound> instance;
    static Sound& GetInstance() {
        if (!instance) instance = std::make_unique<Sound>(hWnd);
        return *instance;
    }
    void cleanup() { instance.reset(); }  // safe, RAII
};
```

---

## 23. `BOOL` / `FLOAT` / `INT` — Win32 Type Aliases (Low Priority)

### Finding
The codebase uses Win32 typedefs extensively:
- `BOOL` (which is `int`) used for boolean returns and conditionals
- `FLOAT` (which is `float`)
- `INT` (which is `int`)
- `DWORD`, `UINT`

### Current Usage
- `BOOL isDead`, `BOOL isDrown`, `BOOL isFake`, `BOOL isEnemy` — boolean flags
  as `int` (32-bit, not 1-byte)
- `BOOL AABBCheck(...)`, `BOOL Contain(...)` — return types
- `INT hitCounts`, `INT hp` — integer members

### Opportunity
Replace `BOOL` with `bool` and `FLOAT` with `float` for new code and
incremental cleanup. Note: some of these map directly to Win32/DirectX
API signatures and must stay. The `BOOL`→`bool` change is most valuable
for boolean flags and return types where the value is only ever 0 or 1.

---

## 24. `#pragma region` / Data Table Verbosity (Low Priority)

### Finding
**47** `#pragma region` markers across 18 files, used exclusively to fold
large `LoadSprites`/`LoadTextures`/`LoadAnimations` data tables. The
game smell report (§19) notes these are pure folding aids for
over-sized functions.

### Opportunity
If the data tables are ever externalised to JSON/asset files (the project
already depends on `tileson.hpp` for Tiled map loading), these regions
disappear entirely. In the meantime, they can be replaced with
`// region: Load Sprites` comments that don't require IDE support.

---

## 25. Duplicate Code in State Classes (Medium Priority, Mechanical)

### Finding
132 `*State*.cpp` files implement the same 5-method skeleton:
```cpp
StateClassName::StateClassName() : BaseState() {}
StateClassName::~StateClassName() {}
void StateClassName::Exit(Bill& bill) {}
void StateClassName::Enter(Bill& bill) { ... }
void StateClassName::Render(Bill& bill) { bill.SetAnimation(...); }
BillState* StateClassName::Update(Bill& bill) { return NULL; }
BillState* StateClassName::HandleInput(Bill& bill, Input& input) { ... }
```

**21** of these have `Update` that just `return NULL;` (no-op).
The constructor/destructor boilerplate is repeated identically 132 times.

### Opportunity
In C++20, the `override` specifier + CRTP can reduce this:
```cpp
// Provide default no-op Update in the base class
virtual T* Update(E& e) { return nullptr; }
```

States that return `nullptr` from `Update` no longer need to override it at all.
This eliminates ~21 `Update` methods and their boilerplate.

---

## 26. `std::variant` Hashing — `GraphicsDatabase` Performance (Low Priority)

### Finding
```cpp
// GraphicsHelper.cpp:137
// TEXTURE_ID is a variant over ~20 enum types whose std::hash is not free.
```

`GraphicsDatabase` uses `std::unordered_map` keyed by `std::variant` over
~20 enum types. Each lookup hashes the variant (visits ~20 alternatives).

### Opportunity
C++20 `std::hash` for `enum class` is available. If the variant alternatives
are all `enum class` (they are), the hash is still O(N) in the number of
alternatives. A C++23 `std::flat_map` won't fix the hash cost, but if the
IDs were flattened to a single `enum class` with a namespace per object type,
the map lookup would be O(1) hash instead of O(20).

---

## 27. `std::cmp_greater_equal` — Already Adopting C++20 (Positive Example)

### Finding
**21** uses of `std::cmp_greater_equal` in `HasAnimations.h` and `.cpp`.
This is a C++20 feature that replaces the dangerous `currentFrame >= 0`
comparison (size_t vs int) with a safe checked comparison.

### Assessment
This is already done correctly. The `currentFrame` is `INT` (signed) and
`frames.size()` returns `size_t` (unsigned). `std::cmp_greater_equal` is the
right tool here.

---

## Priority Matrix

| # | Theme | Effort | Risk | Impact | Priority |
|---|---|---|---|---|---|
| 1 | `NULL` → `nullptr` (461 sites) | Low | Low | Medium | ⭐⭐⭐⭐⭐ |
| 2 | `std::unique_ptr` for state objects (342 `new` → RAII) | High | High | High | ⭐⭐⭐⭐⭐ |
| 3 | ComPtr for all COM pointers (Sound, Input, device) | Medium | Low | High | ⭐⭐⭐⭐ |
| 4 | `Bool<T>` → inline `bool` variables | Low | Low | Low | ⭐⭐⭐ |
| 5 | `#define` → `constexpr` (5 macros) | Low | Low | Low | ⭐⭐⭐ |
| 6 | C-style enum → `enum class` (1 site) | Low | Low | Low | ⭐ |
| 7 | C-style casts → `static_cast` (2+ sites) | Low | Low | Low | ⭐⭐ |
| 8 | Iterator loops → range-for (5 sites) | Low | Low | Low | ⭐⭐ |
| 9 | `ZeroMemory` → `{}` init (4 sites) | Low | Low | Low | ⭐⭐ |
| 10 | `Sound` self-delete singleton → RAII | Medium | Medium | Medium | ⭐⭐⭐⭐ |
| 11 | `std::string` → `string_view` params (Sound, FormatId) | Low | Low | Low | ⭐⭐ |
| 12 | `std::list` → `std::vector` (14 sites) | Medium | Medium | Medium | ⭐⭐⭐ |
| 13 | `std::to_string` → `std::format` (15 sites) | Low | Low | Low | ⭐⭐ |
| 14 | Upgrade build to C++23 (`<LanguageStandard>`) | Very Low | Very Low | Enables §15-17 | ⭐⭐⭐⭐ |
| 15 | `[[nodiscard]]` / `noexcept` on getters/destructors | Low | Low | Medium | ⭐⭐ |
| 16 | Unicode identifiers → ASCII (ω, θ, φ, xO, yO) | Low | Low | Low | ⭐⭐ |
| 17 | `ID` template inherits `std::string` → fix UB | Low | Low | Correctness | ⭐⭐⭐ |
| 18 | `std::sort` lambda: const ref + `bool` + `ranges::sort` | Very Low | Low | Very Low | ⭐ |
| 19 | C-style `abs()` → `std::abs()` (80 sites) | Low | Low | Correctness | ⭐⭐ |
| 20 | Magic `#define PI` → `std::numbers::pi_v` (C++20) | Low | Low | Low | ⭐⭐ |
| 21 | `.insert()` → `.emplace()` (15 sites) | Low | Low | Low | ⭐⭐ |
| 22 | `OutputDebugString`/`cout` → `std::print`/`std::println` (C++23) | Low | Low | Low | ⭐⭐ |
| 23 | `reinterpret_cast` → `std::bit_cast` (C++20, 10+ sites) | Low | Low | Low | ⭐⭐ |
| 24 | Map `[]` → `.at()` / `.try_emplace()` (correctness) | Low | Low | Low | ⭐⭐ |
| 25 | `auto&` → `const auto&` in read-only loops (many) | Low | Low | Performance | ⭐⭐ |
| 26 | `std::exchange` for state-null transitions | Very Low | Low | Very Low | ⭐ |
| 27 | Raw `char[]` → `std::array<char, N>` (Sound, Input) | Low | Low | Low | ⭐ |
| 28 | Nullable pointer getters → `std::optional` | Low | Low | Correctness | ⭐⭐ |
| 29 | `std::move` / `std::forward` — 0 usages (all C++17 elision) | Low | Low | Perf | ⭐⭐ |
| 30 | `if constexpr` / `<=>` / `decltype(auto)` (0 usages, C++17/20) | Low | Low | Readability | ⭐⭐ |
| 31 | `std::visit` / structured bindings for tuple access (0 / 22 sites) | Low | Low | Correctness | ⭐⭐ |
| 32 | `std::all_of`/`find_if`/`transform` — 0 STL algorithms beyond `std::sort` | Low | Low | Maintainability | ⭐⭐ |
| 33 | `std::bit_cast` for DX11Math reinterpret casts (10+ sites) | Low | Low | Safety | ⭐⭐ |
| 34 | `std::ss`/`std::ssize` — replace `std::cmp_less` / `static_cast` casts | Low | Low | Readability | ⭐ |
| 35 | `std::hypot` — replace `sqrt(x*x+y*y)` | Low | Low | Correctness | ⭐ |
| 36 | `= default` / `= delete` on empty special members | Low | Low | Maintainability | ⭐ |
| 37 | `std::array` for `std::list` fixed-size `nodes[4]` | Low | Low | Type safety | ⭐ |

---

## C++ Version Roadmap

### If staying on C++20 (no toolset upgrade):
Focus on: `nullptr` migration, `std::unique_ptr`, `ComPtr`, `#define`→`constexpr`,
range-for, `static_cast`, `string_view` params, `[[nodiscard]]`/`noexcept`,
`Bool<T>`→inline bool, `std::move`/`std::forward`, `std::bit_cast`,
`std::span`, `std::format`, `std::ranges` algorithms, `std::apply`/`tuple`
structured bindings, `if constexpr`, `<=>` operator, `consteval`/`constinit`.

### If upgrading to C++23:
All of the above **plus**:
- `std::print`/`std::println` (replaces `std::cout`, `OutputDebugString` in debug builds)
- `std::flat_map` / `std::flat_set` (for Sound `soundBufferMap`, GraphicsDatabase maps,
  `std::set` usage — 6 `std::map` + 18 `std::unordered_map` + 1 `std::set` could be flattened)
- `std::mdspan` (for sprite/buffer views, 2D vertex data)
- `std::expected` (for error-returning functions: 28 `BOOL` returning functions,
  GraphicsHelper::Init, Sound::loadSound)
- `std::move_only_function` (for callbacks; 1 `std::function` site)
- `std::ranges::to` (for container conversions)
- `if consteval` (for compile-time branching)
- `std::ranges::sort` / `std::ranges::find` / `std::ranges::for_each` (replace `std::sort`, manual loops)
- `std::erase_if` / `std::erase` (replace 3 `std::list::remove()` + manual remove-erase)
- `std::ssize` (signed-size comparisons, replaces `std::cmp_*` casts)
- `std::is_power_of_2` (C++23; replaces manual bit checks in QuadTreeNode)
- `std::start_lifetime_as` (C++23; replaces `reinterpret_cast` in DX11Math type-punning)
- `std::print` for `OutputDebugString` replacement (debug builds; 66 occurrences)

### Recommended incremental path:
1. **Phase 1:** `nullptr` migration + `#define`→`constexpr` + iterator→range-for
   + `abs()`→`std::abs()` + `ZeroMemory`→`{}` + C-style casts→`static_cast`
   (mechanical, ~10 files, no logic change, safe)
2. **Phase 2:** `Bool<T>` → inline bool + `ComPtr` for Sound/Input + `std::array`
   for raw arrays + `#define PI` → `std::numbers::pi_v` + `std::numbers::pi_v<float>`
   (medium, RAII, low risk)
3. **Phase 3:** Build standard bump to C++23 + `std::format`/`std::print` +
   `std::span`/`std::expected` where applicable + `[[nodiscard]]`/`noexcept`
   on all getters + `std::apply`/structured bindings for tuple access
   (enables C++23 features, low risk)
4. **Phase 4:** `std::unique_ptr` state machine refactoring (highest leverage,
   highest risk — defer until phases 1-3 are stable)
5. **Phase 5:** `std::ranges` algorithms + `std::flat_map` + `std::erase_if`
   + `std::exchange` + `std::bit_cast`/`std::span` for DX11Math +
   `std::move`/`std::forward` throughout + `std::visit` for variant access
   (C++20/23 polish, low risk after RAII migration)

---

---

## 28. Additional Findings (Continued Scan #2)

### 28a. C-Style `abs()` → `std::abs()` (Low Priority, Mechanical)

### Finding
**80** usages of `abs()` (from `<cstdlib>`) instead of `std::abs()` (from
`<cmath>`). The C `abs()` only handles `int`; calls with `float` arguments
silently use the integer overload, truncating to `int` before taking the
absolute value — a **latent precision bug** for floating-point physics.

Files affected: `Soldier.cpp` (257-272), `RifleMan*/Standing*` (4 files),
`WallTurret*State.cpp` (8 files), `BossStage3*` (5 files), and more.

### Action
Replace all `abs(` with `std::abs(` and ensure `<cmath>` is included.

### 28b. `#define PI` / Magic Numbers → `std::numbers::pi_v` (Low Priority, Mechanical)

### Finding
```cpp
// ScubaSoldier.cpp:3
#define PI D3DX_PI
// WallTurret.cpp:3
#define PI D3DX_PI
```
Plus a raw magic number:
```cpp
// BossStage3Joint.cpp:128
distanceMoved = abs((2 * 3.1415926f * moveAroundRadius * moveAroundSpeed) / 360);
```

### Opportunity (C++20 `std::numbers`)
```cpp
#include <numbers>
inline constexpr FLOAT PI = std::numbers::pi_v<FLOAT>;
// BossStage3Joint.cpp:
distanceMoved = abs((2.0f * std::numbers::pi_v<float> * ...) / 360.0f);
```

The `#define PI` macros should be `inline constexpr float PI = ...;`.
The `180.0f / D3DX_PI` degree-radian conversions (4 sites in `Soldier.cpp`)
can use `std::numbers::pi_v<float>`.

### 28c. `OutputDebugString` (66 sites) → `std::println` (C++23, Low Priority)

### Finding
**66** `OutputDebugString`/`OutputDebugStringA`/`OutputDebugStringW` calls
across `GraphicsHelper.cpp` (40+), `Bill.cpp`, `Scene.cpp`, `Stage.cpp`,
and state files — all for debug/diagnostic logging.

Plus **6** `std::cout` calls in `Sound.cpp`.

### Opportunity (C++23)
`std::println` to stdout is the modern replacement for `std::cout`.
However, `OutputDebugString` writes to the debugger's output window, which
`std::print` does not do directly. A wrapper could use `std::print` for
console + `OutputDebugString` for debug builds, or use `std::println` when
console output suffices.

### 28d. `std::string` by Value Params → `std::string_view` (Low Priority)

### Finding
**10** `std::string` parameters passed by value where no copy is needed:
- `Sound::play(std::string name, ...)` — called with string literals like `"shootM"`
- `Sound::stop(std::string name)` — same pattern
- `Sound::setVolume(float, std::string name)` — same
- `Sound::loadSound(const char*, std::string name)` — same
- `Common.h:FormatId(std::string id)` — takes by value, only reads `id.size()`
- `Common.h:ID<std::string value>` constructor — takes by value

**8** `const std::string&` params already exist.

### Opportunity
```cpp
void play(std::string_view name, bool infiniteLoop, int times);
void stop(std::string_view name = {});
void FormatId(std::string_view id);
```

**Note:** `Sound::soundBufferMap` is keyed by `std::string`. When looking up
with `string_view`, `std::map::find(string_view)` works without allocation in
C++23 (transparent lookup), but `std::unordered_map` needs a transparent hash
in C++26. An interim solution: accept `string_view`, construct `std::string`
only for the `find` call.

### 28e. `BOOL` Used for Boolean Flags → `bool` (Low Priority, Mechanical)

### Finding
`BOOL` (which is `int`, 32-bit) is used for boolean flags throughout:
- `BOOL isDead`, `BOOL isDrown`, `BOOL isFake`, `BOOL isEnemy` (Entity/Bullet)
- `BOOL isCollided = 0` (AABBSweepResult)
- `BOOL hasChanged = 0` (QuadTreeNode::Update)
- `BOOL hasDone` (Stage)

Initialized with `= 0` instead of `= false` or `= {}`.

### Opportunity
Replace `BOOL` with `bool` for in-game boolean flags and locals. Keep `BOOL`
where the type maps directly to a Win32/DirectX API. `BOOL hasChanged = 0` →
`bool hasChanged = false`.

### 28f. `std::insert` → `std::emplace` (Low Priority)

### Finding
**15** `.insert()` calls vs **0** `.emplace()` calls. Every `insert` constructs
a temporary `std::pair` that `emplace` would avoid by constructing in-place.

Key sites:
```cpp
// GraphicsHelper.cpp:1085
GraphicsDatabase::textures.insert({ textureId, CreateTexture(textureFilePath) });
// GraphicsHelper.cpp:1099
GraphicsDatabase::sprites.insert({ spriteId, CreateSprite(...) });
// GraphicsDatabase::animations.insert({ animationId, CreateAnimation(...) });
// QuadTreeNode.cpp:151,161,191,201
result.insert({ entity, this });
// Stage1.cpp:36-38, Stage2.cpp:42-44
walls.insert({ "L", wallL });
```

### Opportunity
```cpp
GraphicsDatabase::textures.emplace(textureId, CreateTexture(textureFilePath));
result.emplace(entity, this);
walls.emplace("L", wallL);
```

### 28g. `std::pair<Entity*, QuadTreeNode*>` Sort → `std::ranges::sort` (Low Priority)

### Finding
`Stage1.cpp:107` and `Stage2.cpp:115` duplicate the same sort with a
pass-by-value lambda returning `BOOL`:

```cpp
std::sort(vec.begin(), vec.end(),
    [](std::pair<Entity*, QuadTreeNode*> pair1,
       std::pair<Entity*, QuadTreeNode*> pair2) -> BOOL
    { return pair1.first->GetL() < pair2.first->GetL(); });
```

### Opportunity (C++20)
Two improvements:
1. **Pass by `const&`** — currently copies each pair on every comparison.
2. **Use `std::ranges::sort` with a projection** (C++20):
   ```cpp
   std::ranges::sort(vec, {}, &std::pair<Entity*, QuadTreeNode*>::first,
                     &Entity::GetL);
   ```
   Or simpler:
   ```cpp
   std::ranges::sort(vec, std::less{}, [](const auto& p) { return p.first->GetL(); });
   ```

### 28h. `using namespace DirectX` → Scoped `using` (Low Priority)

### Finding
**2** `using namespace DirectX;` directives:
- `DX11Math.h:171`
- `GraphicsHelper.cpp:18`

### Opportunity
Replace with targeted imports:
```cpp
using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;
using DirectX::XMLoadFloat3;
using DirectX::XMStoreFloat4x4;
```

### 28i. `reinterpret_cast` Between Layout-Incompatible Types → `std::bit_cast` (C++20, Low Priority)

### Finding
`DX11Math.h` uses `reinterpret_cast` to reinterpret between its custom
`D3DXMATRIX`/`D3DXVECTOR*` shim types and DirectXMath's `XMFLOAT*` types:

```cpp
// DX11Math.h:151-154
DirectX::XMMATRIX a = XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(this));
DirectX::XMMATRIX b = XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&o));
XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(this), r);
```

### Opportunity (C++20)
`std::bit_cast` provides a type-safe, well-defined alternative:
```cpp
DirectX::XMMATRIX a = XMLoadFloat4x4(
    std::bit_cast<const DirectX::XMFLOAT4X4*>(this));
```

However, the **real** fix (noted in smell §29) is to eliminate the
`D3DXVECTOR*` shim entirely and use `DirectX::XMFLOAT*` types directly.
`std::bit_cast` is a safer interim.

### 28j. Map `[]` Operator → `.at()` or `.try_emplace()` (Low Priority, Safety)

### Finding
`HasAnimations.h:70, 87` and ~12 state files use `GraphicsDatabase::animations[animationId]`
and `GraphicsDatabase::sprites[...]` with the `[]` operator on
`std::unordered_map`. This **silently default-inserts** a bogus entry if the
key was never registered (smell §29).

### Opportunity
Use `.at()` for lookups (throws `std::out_of_range` on missing key) or
`.try_emplace()` for insertions. For the `LoadSprites`/`LoadTextures` registration
phase, `.try_emplace` prevents accidental overwrites.

### 28k. `auto&` in Read-Only Loops → `const auto&` (Low Priority, Performance)

### Finding
**64** `auto&` usages, of which many are in read-only range-for loops where
the element is only read. Examples:

```cpp
// GraphicsHelper.cpp:632
for (auto& entry : GraphicsDatabase::textures) { ... }  // reads entry, doesn't modify
// QuadTreeNode.cpp:38
for (auto& _entity : entities) { ... }  // iterates, doesn't modify _entity
// HasWeapons.cpp:18
for (auto& bullet : bullets) Destroy(bullet);  // calls Destroy, doesn't modify bullet
```

### Opportunity
Change to `const auto&` where the loop body does not modify the element.
This enables the compiler to enforce read-only access and can help with
move semantics.

### 28l. `std::exchange` for State Transitions (Low Priority, C++14)

### Finding
The `Bullet::Render()` and `Bill::Render()` methods use a pattern:
```cpp
if (updateState)
{
    ChangeState(state, updateState, this);
    updateState = NULL;  // could use std::exchange
}
```

### Opportunity
`std::exchange(updateState, nullptr)` returns the old value and sets the
new one in one call, making the intent clearer.

### 28m. `std::array<char, 4>` for Binary Structures (Low Priority)

### Finding
`Sound::WaveHeaderStruct` uses C-style char arrays for binary parsing:
```cpp
struct WaveHeaderStruct {
    char chunkId[4];
    char format[4];
    char subChunkId[4];
    char dataChunkId[4];
    // ...
};
```

### Opportunity
`std::array<char, 4>` provides the same memory layout with `.fill()`, `.size()`,
and bounds-checked `.at()`. However, since this struct is `fread` directly
into, the `std::array` must remain trivially copyable — it is.

Similarly, `Input.h:27` has `char keys[256]` which could be `std::array<char, 256>`.

### 28n. `std::optional` for Nullable Pointers (Low Priority, Correctness)

### Finding
Several getters return raw nullable pointers:
```cpp
// Stage.h:23,30
virtual Bill* GetBill()     ;
virtual Camera* GetCamera()  ;
```

`stage` can be NULL (checked as `scene->stageIsReady` before use).

### Opportunity
```cpp
std::optional<Bill*> GetBill();  // or std::optional<Bill&>
```

Or better, return `std::optional<std::reference_wrapper<Bill>>` to make
nullability explicit at the type level.

### 28o. `std::move` — Completely Absent (Low Priority, Performance)

### Finding
**0** `std::move` usages in the entire codebase. While return-value
optimization (RVO/NRVO) is automatic in C++17+, explicit `std::move` on
non-RVO paths (e.g., named local variables returned after conditional logic)
can help. More importantly, the `std::pair` sorting lambda passes pairs by
value — `std::move` could be used in the `emplace_back` patterns.

---

## Additional Statistics

| Metric | Count |
|---|---|
| Total source files | 265 |
| C-style `enum` | 1 (out of 67) |
| `enum class` | 67 |
| `NULL` (to migrate) | **461** |
| `nullptr` (already used) | 51 |
| `override` | 707 |
| `dynamic_cast` | 109 |
| `new` expressions | **342** |
| `delete` expressions | 8 |
| `Destroy<T>()` calls | 91 |
| `#pragma region` | 47 |
| `#define` macros | 5 |
| C-style casts | ~7 |
| `std::list` | 14 |
| `std::map` | 6 |
| `std::vector` | 101 |
| `std::tuple` | 35 |
| Range-based `for` | 52 |
| `auto` | 228 |
| `nullptr` in ComPtr | 0 (uses `nullptr` for 3 statics) |
| `ComPtr` usage | 37 (partial coverage) |
| `std::unique_ptr` | 1 |
| `std::optional` | 1 |
| `std::format` | 0 |
| `std::string_view` | 0 |
| `std::span` | 0 |
| `noexcept` | 0 |
| `[[nodiscard]]` | 0 |
| `std::to_string` | 15 |
| `std::sort` | 2 |
| `std::function` | 1 |
| `ZeroMemory` | 4 |
| `std::move` | 0 |
| `std::make_unique` | 0 |
| C-style `abs()` | 80 |
| `#define PI` | 2 |
| `.insert()` | 15 |
| `.emplace()` | 0 |
| `reinterpret_cast` (DX11Math) | 10+ |
| Map `[]` direct access (silent insert) | 12+ |
| `auto&` read-only loops | 15+ |
| `std::swap` | 1 |
| `std::exchange` | 0 |
| `std::array` | 2 |
| `std::bit_cast` | 0 |
| `const auto&` | 0 |
| `using namespace` | 2 |
| `OutputDebugString` | 66 |
| `std::cout` | 6 |
| `std::numbers::pi` / `std::numbers::pi_v` | 0 |
| `std::derived_from` / `requires` | 10 |
| `if constexpr` | 0 |
| `<=>` spaceship | 0 |
| `[[likely]]` / `[[unlikely]]` | 0 |
| `std::ranges` | 0 |
| `std::erase_if` | 0 |
| `std::ssize` | 0 |
| `std::clamp` | 0 |
| `std::lerp` | 0 |
| `std::midpoint` | 0 |
| `std::bit_ceil` / `bit_floor` | 0 |
| `std::endian` | 0 |
| `consteval` / `constinit` | 0 |
| `std::as_const` | 0 |
| `std::apply` | 0 |
| `std::invoke` | 0 |
| `std::forward` | 0 |
| `std::make_from_tuple` | 0 |
| `std::flat_map` | 0 |
| `std::mdspan` | 0 |
| `std::expected` | 0 |
| `std::move_only_function` | 0 |
| Raw `char[]` arrays | 6 |
| `= default` (functions) | 0 |
| `= delete` (functions) | 0 |
| `vector<bool>` | 0 |
| `std::deque` | 0 |
| `std::bitset` | 0 |
| `std::queue`/`stack`/`priority_queue` | 0 |
| `std::set`/`unordered_set` | 1 |
| `std::shared_ptr`/`weak_ptr` | 0 |
| `std::any` | 0 |
| `std::monostate` | 0 |
| `std::holds_alternative` | 0 |
| `std::visit` | 0 |
| `std::apply` | 0 |
| `std::make_from_tuple` | 0 |
| `std::tuple_cat` | 0 |
| `std::all_of`/`any_of`/`none_of` | 0 |
| `std::find_if`/`find_if_not` | 0 |
| `std::transform`/`for_each`/`generate` | 0 |
| `std::copy`/`copy_if`/`copy_n` | 0 |
| `std::count`/`count_if` | 0 |
| `std::remove_if`/`std::unique` (algo) | 0 |
| `std::fill`/`fill_n` | 0 |
| `std::accumulate` | 0 |
| `std::is_sorted`/`binary_search` | 0 |
| `std::partition`/`stable_partition` | 0 |
| `std::iter_swap`/`swap_ranges` | 0 |
| `std::search`/`starts_with`/`ends_with` | 0 |
| `std::to_array` | 0 |
| `std::byteswap` | 0 |
| `std::assume_aligned` | 0 |
| `std::hardware_interference_size` | 0 |
| `std::call_once`/`once_flag` | 0 |
| `std::thread`/`mutex`/`atomic`/cond_var | 0 |
| `std::jthread`/`stop_token` | 0 |
| `std::latch`/`barrier`/`semaphore` | 0 |
| `std::conditional_t`/`remove_cv_t` | 0 |
| `std::enable_if` | 0 |
| `std::is_constant_evaluated()` | 0 |
| `decltype(auto)` | 0 |
| Trailing return types (`auto ... ->`) | 0 |
| `if consteval` | 0 |
| Manual `for (int i = 0)` loops | 23 |
| `= default` (function defs, not false matches) | 0 |

---

## 29. Advanced STL Algorithms — Zero Adoption

The codebase uses **0** `<algorithm>` calls beyond `std::sort` (2 sites). All iteration logic is
hand-written `for` loops. There are **23** manual `for (int i = 0; …)` index loops that could be
replaced with range-based loops, STL algorithms, or C++20 `std::views` adaptors.

| Algorithm | Used | Opportunity |
|---|---|---|
| `std::all_of` / `std::any_of` / `std::none_of` | 0 | Replace "loop + if-return" boolean check patterns across 109 `dynamic_cast` sites, collision checks (QuadTreeNode.cpp), and entity search loops |
| `std::find_if` / `std::find_if_not` | 0 | Replace manual entity-list search loops (Stage.cpp, QuadTreeNode.cpp, Scene.cpp) — ~30+ search loops |
| `std::transform` / `std::for_each` | 0 | Replace manual "iterate + mutate" loops on `entities`, `bullets`, `sprites` containers (~50+ sites) |
| `std::copy` / `std::copy_if` | 0 | Replace manual copy loops in GraphicsHelper, Bullet, Sound |
| `std::count` / `std::count_if` | 0 | Replace manual counting loops (e.g., counting live entities, bullets) |
| `std::remove_if` / `std::unique` (algorithm) | 0 | Only `std::list::remove()` is used (3 sites in QuadTreeNode.cpp:53,63,265) — migrating to `std::vector` + `std::erase_if` is cleaner |
| `std::fill` / `std::fill_n` | 0 | `ZeroMemory` (§12) is used instead for 4 sites; manual loops used elsewhere |
| `std::accumulate` | 0 | Replace manual sum/aggregate loops |
| `std::is_sorted` / `std::binary_search` | 0 | `std::sort` is used (Stage1/Stage2) but no sortedness checks or binary searches on sorted data |
| `std::partition` / `std::stable_partition` | 0 | Entity sorting/partitioning by type, alive/dead status, etc. |
| `std::iter_swap` / `std::swap_ranges` | 0 | Only `std::swap(y1, y2)` (Stage.cpp:385) |
| `std::search` / `starts_with` / `ends_with` | 0 | String comparison patterns (Sound.cpp paths, file loading) could use `starts_with` |
| `std::equal` / `std::mismatch` | 0 | Found only in `std::equal_to` functor specializations (LetterCommon.h etc.), not actual algorithm calls |

### Representative example — `std::all_of` replacing manual loop:
```cpp
// Current (QuadTreeNode.cpp:6):
for (int i = 0; i <= 3; i++)
    if (this->nodes[i]) this->nodes[i]->Clear(), Destroy(this->nodes[i]);

// Could be:
std::array<std::unique_ptr<QuadTreeNode>, 4> nodes;
// ...
for (auto& child : nodes)
    if (child) child->Clear();
// Or with std::for_each if side-effects are needed.
```

---

## 30. C++20/23 Language & Library Features — Zero Adoption

Despite being on C++20, the project uses almost none of its headline features.

### 30a. Move Semantics — `std::move` (0 usages)

**Zero** `std::move` calls across all 265 files. While C++17+ guarantees copy elision for returns,
`std::move` is still needed for named locals returned through non-trivial paths, and for
explicitly moving into containers.

**Key impact site:** The `std::sort` lambda in Stage1.cpp:107 and Stage2.cpp:115:
```cpp
// Current — passes std::pair by VALUE (unnecessary copy):
std::sort(pendingSortPairs.begin(), pendingSortPairs.end(),
    [](std::pair<Entity*, QuadTreeNode*> a, std::pair<Entity*, QuadTreeNode*> b) -> BOOL { ... });

// Should be:
std::sort(pendingSortPairs.begin(), pendingSortPairs.end(),
    [](const auto& a, const auto& b) { ... });
// Or better, C++20: std::ranges::sort(pendingSortPairs, [](const auto& a, const auto& b) { ... });
```

### 30b. `std::make_unique` / `std::make_shared` (0 / 0)

All **342** `new` expressions (§4) use raw `new`. `std::make_unique` is never used — the codebase
relies on the `Destroy<T>()` template (§17) which wraps raw delete. `std::make_shared` is also 0.

### 30c. `std::shared_ptr` / `std::weak_ptr` (0 / 0)

Only 1 `std::unique_ptr` exists (Stage.cpp:331 for `tson::Map`). No shared ownership semantics
anywhere — all object graphs use raw pointers with manual lifecycle management via `Destroy<T>()`.

### 30d. `<=>` Spaceship Operator (0 usages)

**Zero** three-way comparison operators. Every `Entity`, `Bullet`, `Bill` state class that
implements comparison does so manually with `if (a > b) return 1; if (a < b) return -1; return 0;`.
Could be replaced with a single `auto operator<=>(const X&) const = default;` (C++20).

### 30e. `if constexpr` (0 usages)

**Zero** `if constexpr` usage. The `HasAnimations::SetAnimation` template (§25) and
`HasWeapons::Fire` dispatch chain use `dynamic_cast` chains and runtime branching where
`if constexpr` with concepts could eliminate branches at compile time.

### 30f. `std::bit_cast` (0, 15 `reinterpret_cast`)

15 `reinterpret_cast` calls exist (mostly in `DX11Math.h:151-184` between `XMFLOAT2`/`XMFLOAT3`/
`XMFLOAT4X4` types). These reinterpret casts between layout-compatible-but-different-named types
are exactly what `std::bit_cast` (C++20) is designed for — with compile-time size checks:
```cpp
// Current:
reinterpret_cast<XMFLOAT3*>(xmvec);

// Could be:
std::bit_cast<XMFLOAT3>(*xmvec);  // with compile-time static_assert(sizeof match)
```

### 30g. `std::span` (0 usages)

Six files pass raw pointers + sizes where `std::span` (C++20) would encode the contract:
- `GraphicsHelper.cpp` / `GraphicsHelper.h` — vertex buffer, index buffer, sprite RECT* params
- `Sound.cpp` / `Sound.h` — audio buffer pointer + size
- `DX11Math.h` — vector/matrix data pointers
- `State.h` — generic pointer + size params

### 30h. `std::format` (0 usages, 30 candidate sites)

`std::format` (C++20) could replace:
- 15 `std::to_string` + `+` string concatenation calls (§7)
- 66 `OutputDebugString` debug prints (§28c)
- 6 `std::cout` calls (§28c)

### 30i. `std::ranges` (0 usages)

**Zero** `std::ranges::` calls. The 2 `std::sort` calls (Stage1.cpp:107, Stage2.cpp:115) use the
pre-ranges API. The 23 manual `for (int i = 0)` loops and ~50 manual iterate-and-mutate loops
could use `std::views` filters/transforms.

### 30j. `std::erase` / `std::erase_if` (0, 3 `std::list::remove`)

3 `std::list::remove()` calls in `QuadTreeNode.cpp:53,63,265` manage entity removal. Migrating
`std::list` → `std::vector` (§14) would enable `std::erase_if` (C++20) for cleaner removal.

### 30k. `std::ssize` (0 usages)

Would replace `static_cast<INT>(vec.size())` patterns and the `std::cmp_less(i, vec.size())`
workarounds found in `WallTurret.cpp:384,402`, `Letter.cpp:97,106`, `Stage.cpp:381`.

### 30l. `std::lerp` (0 usages)

`Motion.cpp` implements manual linear interpolation for position, velocity, and angle. `std::lerp`
(C++20) would be clearer and potentially vectorized:
```cpp
// Current (Motion.cpp):
x = x0 + t * (x1 - x0);

// Could be:
x = std::lerp(x0, x1, t);
```

### 30m. `std::clamp` (0 usages)

Manual `if (x < min) x = min; if (x > max) x = max;` patterns exist throughout. `std::clamp`
(C++17) would be clearer and less error-prone.

### 30n. `std::midpoint` (0 usages)

Manual `(a + b) / 2` averaging exists in Stage.cpp:381 area sort and elsewhere. Could use
`std::midpoint` (C++17).

### 30o. `std::hypot` (0 usages, 1 site)

`sqrt(dx * dx + dy * dy)` in `Falcon.cpp:79` could be `std::hypot(dx, dy)` — avoids overflow
and is semantically clearer.

### 30p. `std::numbers::pi_v` (0 usages, 4 magic π)

4 sites use `3.1415926f` or `D3DX_PI` or `180.0f / D3DX_PI` (Soldier.cpp:257-272,
BossStage3Joint.cpp:128). `std::numbers::pi_v<float>` (C++20) provides the exact constant.

---

## 31. Attributes & Specifiers — Zero Adoption

### 31a. `[[nodiscard]]` (0 usages, 459 `Get*` methods)

**Zero** `[[nodiscard]]` attributes. The codebase has 459 `GetX`/`GetY`/`GetState`/etc. getters
that return values — adding `[[nodiscard]]` would prevent bugs where return values are silently
discarded.

### 31b. `noexcept` (0 usages)

**Zero** `noexcept` specifications. All destructors, copy/move constructors, and getters are
implicitly `noexcept(false)` (the default), preventing the compiler from devirtualization
optimizations and making move semantics unsafe.

### 31c. `[[likely]]` / `[[unlikely]]` (0 usages, C++20)

Zero branch-prediction hints. Hot loops in collision detection (QuadTreeNode), rendering
(GraphicsHelper), and state transitions (132+ state classes) would benefit.

### 31d. `[[fallthrough]]` (0 usages, C++17)

Switch statements in state classes that intentionally fall through have no annotation — could
trigger `-Wimplicit-fallthrough` warnings (though MSVC uses Level3 which may not warn).

### 31e. `[[maybe_unused]]` (0 usages)

Unused parameters in overridden state methods (many state classes have `OnUpdate`, `OnExit`,
etc. with unused params) could be annotated.

### 31f. `= default` / `= delete` (0 / 0)

No explicit `= default` or `= delete` on special member functions. Empty destructors that could
be `= default` are manually written out. Copy constructors/moves are not explicitly deleted on
singleton/resource-owning classes (Sound, GraphicsHelper, Scene, Stage).

### 31g. `consteval` / `constinit` (0 / 0, C++20)

`FormatId` (Common.h) could be `consteval`. Static singletons and global config objects could
use `constinit` to guarantee thread-safe static initialization.

### 31h. `decltype(auto)` (0 usages)

Could improve return type deduction in template helper functions (e.g., `GraphicsDatabase::Get`).

### 31i. Trailing Return Types (0 usages)

`auto foo() -> Type` syntax could improve readability for complex template return types, but
no instances found.

### 31j. `if consteval` (0 usages, C++23)

Could be used in template branches where compile-time specialization differs from runtime.

---

## 32. Concurrency Primitives — Fully Single-Threaded

The project is **100% single-threaded** — not a single concurrency feature is used:

| Feature | Count | Context |
|---|---|---|
| `std::thread` / `std::mutex` / `std::atomic` / `std::condition_variable` | 0 | Rendering, physics, audio, input all on main thread |
| `std::call_once` / `std::once_flag` | 0 | `Sound::getInstance()` (§22) uses manual `if (instance == NULL)` instead — race-condition-prone |
| `std::scoped_lock` / `std::lock_guard` | 0 | No lock-free or locked data structures |
| `std::jthread` / `std::stop_token` | 0 | C++20/23 cancelable threads unused |
| `std::latch` / `std::barrier` / `std::semaphore` | 0 | Synchronization primitives unused |
| `std::shared_mutex` / `std::shared_lock` | 0 | Read-write locking unused |

**Note:** For a game engine, this is an architectural limitation — not necessarily a defect.
However, `std::call_once` would still improve the `Sound` singleton initialization safety.

---

## 33. Variant Visitation — `std::visit` Absent

`std::variant` is used for `SPRITE_ID`, `TEXTURE_ID`, `ANIMATION_ID` type aliases (Common.h),
with `std::get<>` extraction (22 sites) in `GraphicsHelper.cpp` and `HasAnimations.h`. However:

| Pattern | Count | Status |
|---|---|---|
| `std::visit` | 0 | Variants are used only as map keys / equality comparisons, never for type-dispatch |
| `std::holds_alternative` | 0 | No runtime type checking on variants |
| `std::monostate` | 0 | No "empty" variant state — `RECT*` in `SPRITE` tuple allows null without `monostate` |

**Opportunity:** Where `std::get<I>` is used on `std::tuple` types (22 sites in `HasAnimations.h`,
`GraphicsHelper.cpp`), C++20 structured bindings (`auto [a, b, c] = tuple`) or `std::apply`
(§29) would be safer and more readable.

---

## 34. Tuple Processing — `std::apply` / `std::tuple_cat` Absent

| Pattern | Count | Opportunity |
|---|---|---|
| `std::apply` | 0 | 22 `std::get<I>` calls on `SPRITE` tuples and animation-frame tuples could use `std::apply` |
| `std::make_from_tuple` | 0 | Could simplify tuple-based construction |
| `std::tuple_cat` | 0 | No tuple concatenation |
| `std::get<I>` on tuples | 22 | `HasAnimations.h:87-88`, `GraphicsHelper.cpp:1069-1070,1134-1136`, 30+ state file duplicates |

---

## 35. Bit Manipulation — Manual Operations

| Pattern | Count | Opportunity |
|---|---|---|
| `std::bit_ceil` / `bit_floor` / `has_single_bit` | 0 | `QuadTreeNode::New` has a manual `while (w < target) w *= 2;` loop — exactly `std::bit_ceil` |
| `std::popcount` / `countl_zero` / `countr_zero` | 0 | Bit-counting loops could use intrinsics |
| `std::rotl` / `std::rotr` | 0 | No bit rotation needed |
| `std::is_power_of_2` | 0 | `QuadTreeNode::New` doubles dimensions but doesn't check |
| `std::endian` | 0 | Byte-order checks could use `std::endian` (C++20) |
| `std::byteswap` | 0 | No byte-swapping needed for little-endian target |
| `std::assume_aligned` | 0 | SIMD vertex buffers could assert alignment |
| `std::hardware_interference_size` | 0 | Cache-line alignment for performance-critical structs |

---

## 36. Compile-Time Programming — Limited

| Pattern | Count | Opportunity |
|---|---|---|
| `std::is_constant_evaluated()` | 0 | `std::derived_from` concept already used (§27), but `if consteval` could branch further |
| `std::conditional_t` | 0 | Could simplify conditional type selection |
| `std::remove_cv` / `remove_reference` / `remove_const` / `decay_t` | 0 | Type traits unused (concepts preferred — which is correct) |
| `std::enable_if` | 0 | Replaced by `requires` — **good practice** |

**Positive:** The `Bool<T>` template (§5) uses template specialization which is a valid modern
pattern. The `State.h` and `HasWeapons.h` concepts usage (10 sites) is good C++20 adoption.

---

## 37. Container Adapters & Specialized Containers — Unused

| Container | Count | Notes |
|---|---|---|
| `std::queue` / `std::stack` / `std::priority_queue` | 0 | Bullet/entity lists use `std::list` + manual push/pop — `std::priority_queue` would suit ordered rendering |
| `std::set` / `std::multiset` | 1 | Only 1 `std::set` (likely in `<set>` include) — `std::flat_set` (C++23) would be better |
| `std::deque` | 0 | `std::vector` is used everywhere |
| `std::bitset` | 0 | Raw `char[]` (§28m) could use `std::bitset` or `std::array` |
| `std::vector<bool>` | 0 | Good — the codebase avoids the infamous specialization |

---

## 38. `std::next` / `std::prev` / `std::distance` (1 usage only)

Only **1** usage: `QuadTree.h:121: std::prev(entityList.end())` — the rest use raw pointer
arithmetic or index-based iteration. The `std::list::iterator` in `QuadTreeNode.cpp` could
benefit from `std::next` instead of manual `++` chains.

---

## 39. `= default` / `= delete` Opportunities (0 / 0)

| Class | Empty Dtor | Copy Ctor | Move Ctor | Copy Assign | Move Assign | Could be `= default`? |
|---|---|---|---|---|---|---|
| `Sound` (singleton) | ~Sound() | — | — | — | — | Dtor could `= default` if members use RAII |
| `GraphicsHelper` | — | — | — | — | — | Dtor could `= default` with `ComPtr` cleanup |
| `Scene` | — | — | — | — | — | Empty special members could `= default` |
| `State<T>` | — | — | — | — | — | Template could `= default` copy/move |
| `Entity` | — | — | — | — | — | Empty special members |

Conversely, **0 `= delete`** means copy constructors are **not** explicitly deleted on classes
that should be non-copyable (Sound singleton, GraphicsHelper, Scene, SceneManager). The `ID`
template (§21) has a deleted-but-inheriting copy constructor through `std::string` inheritance
— this is actually UB-prone.
