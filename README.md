# UnityMonoLoader
MonoLoader

## Disable anti-cheat classes
```с++
BOOL AntiCheatOff = MonoInject.InjectMono("Sunrise.exe", "mono.dll", L"Assembly-CSharp.dll", "CodeStage.AntiCheat.Detectors", "InjectionDetector", "StopDetection");
BOOL AntiCheatsOff1 = MonoInject.InjectMono("Sunrise.exe", "mono.dll", L"Assembly-CSharp.dll", "CodeStage.AntiCheat.Detectors", "SpeedHackDetector", "StopDetection");
```

![image](https://user-images.githubusercontent.com/79174725/135528251-ddf314fc-8393-4977-87cc-f0cf4eac0c03.png)
