
## Sprata Base 퍼즐 스테이지 구현 과제 - README

### 1. 프로젝트 소개

이번 과제는 언리얼 엔진 5 C++를 활용하여 퍼즐 스테이지를 구현하는 프로젝트입니다. **회전 발판**과 **이동 플랫폼**을 C++ 코드로 직접 제작하고, 언리얼 엔진 기능들을 활용했습니다. **Tick 함수** 기반의 움직임 제어, **리플렉션 시스템**을 통한 에디터 변수 노출 및 실시간 수정 기능을 구현하여 레벨 디자인 효율성을 높였습니다.


### 2. 구현 기능

본 프로젝트에서 구현한 주요 기능은 다음과 같습니다.

#### 2.1. 2종 Actor 클래스 구현

*   **`AMovingActor` (이동 플랫폼):**
    *   StaticMeshComponent 기반의 이동 플랫폼 액터.
    *   **4가지 이동 모드**: Random Linear, Circular, Zig Zag, Forward-Backward.
    *   Tick 함수에서 이동 모드별 위치 업데이트 로직 구현.
    *   리플렉션 시스템을 통한 이동 속도, 범위, 모드 에디터 실시간 변경.

*   **`ARotatingActor` (회전 발판):**
    *   StaticMeshComponent 기반의 회전 발판 액터.
    *   Tick 함수와 `AddActorLocalRotation()`을 이용한 회전 구현.
    *   **3가지 행동 모드**: Disappear, MeshChange, RotationChange.
    *   타이머를 활용한 행동 모드별 로직 구현 (도전 과제 1번).
    *   리플렉션 시스템을 통한 회전 속도, 행동 모드, 카운트다운 시간 등 에디터 실시간 변경.

#### 2.2. Tick 함수 기반 동적 Transform 변경

*   **회전 기능 (`ARotatingActor`):**  `AddActorLocalRotation()`을 Tick 함수에서 호출하여 매 프레임 회전. `RotationSpeed` 변수로 속도 조절 및 에디터 수정.
*   **이동 기능 (`AMovingActor`):** Tick 함수에서 이동 모드에 따라 위치 업데이트. Random Linear, Circular, Zig Zag, Forward-Backward 4가지 모드 구현. `MoveSpeed`, `MaxRange` 변수 에디터 수정.
*   **프레임 독립성:**  `DeltaTime`을 활용하여 프레임 속도 무관한 일정한 움직임 보장.

#### 2.3. 리플렉션 시스템

*   `UPROPERTY` 매크로를 사용하여 주요 변수 리플렉션 시스템 등록.
*   `EditAnywhere`, `BlueprintReadWrite`, `Category` 지정자를 통한 에디터 Details 패널 편집 및 블루프린트 접근.
*   에디터 변수 변경 시 게임 내 액터 동작에 실시간 반영 확인.

#### 2.4. 도전 과제 구현 (선택)

*   **도전 과제 1번 - 시간 제한 및 카운트다운:** `ARotatingActor`에 행동 모드 및 카운트다운 기능 추가. Disappear, MeshChange, RotationChange 모드 구현. `FTimerHandle` 및 `GetWorldTimerManager().SetTimer()` 활용.
*   **도전 과제 2번 - 랜덤 퍼즐 생성:**  미구현. 향후 `SpawnActor` 및 `FMath::RandRange` 활용하여 확장 예정.

### 3. 실행 방법

1.  GitHub Repository에서 프로젝트 다운로드.
2.  언리얼 엔진 5 에디터 실행.
3.  `.uproject` 파일로 프로젝트 열기.
4.  `Content/Levels` 폴더의 레벨 파일 실행 (또는 새 레벨 생성).
5.  `AMovingActor`, `ARotatingActor` 액터를 레벨에 배치.
6.  Details 패널에서 액터 변수 (속도, 모드, 범위 등) 설정.
7.  에디터 플레이 버튼 클릭 후 게임 실행.
8.  액터 동작 확인 및 Details 패널에서 변수 실시간 수정 확인.


### 4. 파일 구조

```
SprataBase/
├── Content/
│   ├── Levels/
│   │   └── 퍼즐스테이지레벨.umap
│   ├── SprataBase/
│   │   ├── Blueprints/
│   │   ├── Maps/
│   │   ├── Meshes/
│   │   ├── Materials/
│   │   └── Widgets/
│   └── ...
├── Source/
│   ├── SprataBase/
│   │   ├── Private/
│   │   │   ├── MovingActor.cpp
│   │   │   └── RotatingActor.cpp
│   │   └── Public/
│   │       ├── MovingActor.h
│   │       └── RotatingActor.h
│   └── SprataBase.Build.cs
├── SprataBase.uproject
└── README.md
```

