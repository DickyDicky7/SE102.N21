# NESContra

# **NHẬP MÔN PHÁT TRIỂN GAME**

## **Đề tài: Lập trình Game Contra bằng DirectX 11**

> Dự án ban đầu được viết bằng DirectX 9 (D3DX9 + `ID3DXSprite`) và sau đó đã
> được chuyển sang DirectX 11. `src/Abstractions/Graphics/DX11Math.h` là lớp
> tương thích còn lại của lần chuyển đó: nó dựng lại các kiểu toán học của D3DX9
> (`D3DXVECTOR2/3`, `D3DXMATRIX`) trên nền DirectXMath.

## Lớp: SE102.N21

Nhóm sinh viên thực hiện:

| Tên               | MSSV     |
| ----------------- | -------- |
| Phạm Tuấn Anh     | 21520147 |
| Phan Văn Minh     | 21522345 |
| Nguyễn Đức Phương | 21521307 |

### **Các kĩ thuật đã được áp dụng trong đồ án:**

- Phân hoạch không gian phân cấp: QuadTreeNode + BVH hai tầng (TLAS/BLAS)
- AABB Collision (kèm swept AABB cho broadphase)
- Tiled Map
- Scrolling
- Sprite - Animation
- Component
- Physic
- Basic Math
- DirectX 11: pipeline biến đổi tường minh (local → world → view → clip space)
- DirectX 11: vẽ sprite theo lô bằng instancing (instanced sprite batching)
- DirectX 11: shader HLSL được biên dịch bằng FXC lúc build (`.cso`)
- DirectX 11: nạp texture bằng WIC (hỗ trợ BMP/PNG, có alpha)
- Fixed timestep: logic game chạy ở đúng 60 Hz theo đồng hồ thực
  (`QueryPerformanceCounter`), tách rời khỏi tốc độ quét của màn hình — nên game
  chạy đúng tốc độ trên cả màn hình 60 Hz, 120 Hz hay 144 Hz

## Cấu trúc thư mục

Mã nguồn được chia theo **đối tượng**: file `.h` và `.cpp` của cùng một class nằm
cạnh nhau, và các state của một đối tượng nằm trong thư mục con `<Đối tượng>State`.
Cây thư mục này trùng khớp với cây filter trong Solution Explorer của Visual Studio.

```
src/
  main.cpp                    vòng lặp game (fixed timestep) + cửa sổ Win32
  Common.h                    header dùng chung toàn dự án
  Abstractions/
    Graphics/                 GraphicsHelper (back end D3D11), DX11Math, 4 file .hlsl,
                              GraphicsDatabase, Has{Sprites,Textures,Animations}
    Object/                   Entity, State, Motion, Enemy, CollidableEntity, Terrain
    Space/                    QuadTreeNode, AABB, BVH, BLAS, TLAS,
                              SpatialPartitioning
    Device/                   Input (DirectInput 8), Sound (DirectSound 8)
  Objects/
    Bill/                     Bill.h|.cpp, BillCommon.h
      BillState/              22 state của nhân vật chính
    Bullet/, Camera/, Explosion/, Fire/, Item/, Letter/,
    RockFall/, RockFly/, Scene/, Bridge/, TestingEntity/
    Enemies/
      AirCraft/, Cannon/, Falcon/, ScubaSoldier/, Soldier/, WallTurret/,
      RifleMan/{HideOnBush,Standing}/,
      BossStage1/{FinalBossStage1,GunBossStage1}/,
      BossStage3/{BossStage3Gate,BossStage3Hand,BossStage3Head}/
    Terrains/                 TerrainBlock, TerrainStage1, TerrainStage2
  Stages/                     Stage, Stage1, Stage2
third_party/tileson/          thư viện header-only đọc Tiled map (.json)
Resources/                    texture, sound, font, map — copy sang output khi build
```

Các header được `#include` bằng **tên trần** (`#include "Bill.h"`) từ bất kì đâu:
mọi thư mục chứa header đều nằm trên include path, khai báo một lần ở property
`GameIncludeDirs` trong `NESContra.vcxproj`. Khi thêm một thư mục mới có chứa
header, nhớ thêm nó vào đó.

> **Lưu ý:** mọi file `.cpp` trong `src/` đều được biên dịch. Số code chết trước
> đây (`QuadTree` / `QuadTreeContainer`, `AirCraft/AirCraftState/`,
> `Fire/FireState/`) đã bị xoá, nên cây thư mục ở trên đúng bằng những gì thực sự
> đang chạy.

## Các màn chơi

### 1) Màn 1: Jungle

<img src="https://github.com/DickyDicky7/SE102.N21/blob/develop/Resources/images/stage1_img.png" alt="Getting Started" width="500" height="500">

### 2) **Màn 2: Water Fall**

<img src="https://github.com/DickyDicky7/SE102.N21/blob/develop/Resources/images/stage2_img.png" alt="Getting Started" width="500" height="500">

## Cách cài đặt

- Cài đặt Visual Studio 2022, chọn workload **Desktop development with C++**
- **Không** cần cài `DXSDK_Jun10.exe` nữa. Sau khi chuyển sang DirectX 11, mọi
  thứ dự án cần (`d3d11`, `dxgi`, `d3dcompiler`, `DirectXMath`, WIC) đều đã có
  sẵn trong Windows SDK đi kèm Visual Studio
- Trong quá trình build, chọn **x64 Debug** (hoặc **x64 Release**)
- Hoặc build từ dòng lệnh, tại thư mục gốc của dự án:

```powershell
.\build.ps1                    # Debug|x64
.\build.ps1 -Config Release    # Release|x64
```

FXC sẽ biên dịch 4 file `.hlsl` thành `.cso` và một post-build target sẽ copy
`Resources/` cùng các file shader sang thư mục output, nên có thể chạy
`NESContra.exe` trực tiếp từ đó.

## Link video demo

[Google Drive](https://drive.google.com/drive/folders/1F71OwPAA1f4lZqMabdDLTNa0QyQlpCI1?usp=sharing)
