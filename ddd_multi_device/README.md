# DDD 多设备产线控制演示（device_ddd）

面向 **X 大型装备**的 DDD（领域驱动设计）+ 六边形架构（Hexagonal）演示工程，
覆盖：多层级产线下发、逐级上报、富就绪工序、故障影响策略与领域事件。

需求见 [`docs/01-requirement`](docs/01-requirement)，编码设计见
[`docs/02-coding-design`](docs/02-coding-design)。

## 架构分层

```
infrastructure/  接入层：NodeStore(部署节点)、RpcRouter(IRpc适配)、EventBus(IEventSink适配)
     ▲ 依赖倒置：实现领域端口
application/     PlantController：上位机编排用例，实现 IUpstream(上报汇聚)
     ▼ 调用领域
domain/          领域层：HierNode(聚合/状态机)、DeviceModule(状态机+传感映射)、
                 Result/Id 值对象、IUpstream/IRpc/IEventSink 端口 —— 无框架依赖
```

- **下行 RPC**：上层通过 `IRpc` 驱动下一层（`advance/start/finish`）。
- **上行 Pub/Sub**：状态/故障通过 `IUpstream` 逐级上报至上位机。
- **领域事件**：`HierNode` 在 `Alarm/ModuleStateChanged/ModuleReady/FaultCleared`
  时发布到 `IEventSink`(`EventBus`)，供 SCADA/日志/告警中心订阅（解耦扩展单位）。
- **传感映射**：`DeviceModule`（`device_module.hpp/.cpp`）实现文档 §5.1/§5.2
  的状态机与 `SENSOR` 只读映射，非法迁移返回 `Result::failure`。

## 构建与运行

```bash
cmake -S . -B build          # 或 ./scripts/build.sh
cmake --build build          # 或 ./scripts/build.sh
./build/device_ddd           # 三个演示场景
```

## 演示场景

1. **传感映射**：加工台(OPERATOR) 被 温度传感(SENSOR) 的 `NORMAL` Guard 守护开工；
   传感失联(OUT_OF_RANGE) 阻断预热，非法迁移被拒。
2. **场景A 正常产线**：上位机逐层 RPC 下发 上电→预热→自检→到位→就绪→开工。
3. **场景Fault 故障**：非关键设备告警不阻断；关键硬故障(BLOCKING) 阻断并逐级上报。

## 目录结构

```
src/
├─ main.cpp                     # 组合根 + 演示场景
└─ ddd/
   ├─ domain/
   │  ├─ core/    id.hpp, result.hpp
   │  ├─ events/  event.hpp, event_bus.hpp(IEventSink 端口)
   │  ├─ module/  device_module.hpp/.cpp       # 状态机+传感映射(Result)
   │  ├─ port/    rpc.hpp, pubsub.hpp
   │  └─ process/ node.hpp/.cpp, model.hpp, impact.hpp
   ├─ application/ plant_controller.hpp
   └─ infrastructure/
      ├─ messaging/ event_bus.hpp              # IEventSink 实现(EventBus)
      └─ runtime/  store.hpp                    # NodeStore + RpcRouter
```