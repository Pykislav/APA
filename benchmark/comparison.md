## **APA performance comparison with other popular lightweight C++ big integer libraries in Github**

Compiler : **g++**


apa::bint limb base = 2<sup>64</sup>

## **Factorial of (400) - Average (less is better)**

| Library | microseconds |
| ------- | ----------- |
| [boost::multiprecision::cpp_int](https://github.com/boostorg/multiprecision) | 14 μs |
| [apa::bint](https://github.com/mrdcvlsc/APA) | 18 μs |
| [InfInt](https://github.com/sercantutar/infint) | 135 μs |


apa::bint limb base = 2<sup>64</sup>

## **Factorial of (25000) - Average (less is better)**

| Library | microseconds |
| ------- | ----------- |
| [boost::multiprecision::cpp_int](https://github.com/boostorg/multiprecision) | 38382 μs |
| [apa::bint](https://github.com/mrdcvlsc/APA) | 40801 μs |
| [InfInt](https://github.com/sercantutar/infint) | 1392656 μs |

[BigNumber](https://github.com/Limeoats/BigNumber) - not included, very slow at larger factorials
## **fibonacci of (300) - Average (less is better)**


| Library | microseconds |
| ------- | ----------- |
| [apa::bint](https://github.com/mrdcvlsc/APA) | 5 μs |
| [boost::multiprecision::cpp_int](https://github.com/boostorg/multiprecision) | 8 μs |
| [InfInt](https://github.com/sercantutar/infint) | 27 μs |
| [BhimInteger](https://github.com/kothariji/BhimIntegers) | 99 μs |
| [BigInt](https://github.com/faheel/BigInt) | 337 μs |
| [BigNumber](https://github.com/Limeoats/BigNumber) | 530 μs |

## **fibonacci of (6000) - Average (less is better)**


| Library | microseconds |
| ------- | ----------- |
| [apa::bint](https://github.com/mrdcvlsc/APA) | 218 μs |
| [boost::multiprecision::cpp_int](https://github.com/boostorg/multiprecision) | 276 μs |
| [InfInt](https://github.com/sercantutar/infint) | 2816 μs |
| [BhimInteger](https://github.com/kothariji/BhimIntegers) | 18051 μs |
| [BigNumber](https://github.com/Limeoats/BigNumber) | 108936 μs |
| [BigInt](https://github.com/faheel/BigInt) | 148584 μs |


## System Runner

```
Architecture:                       x86_64
CPU op-mode(s):                     32-bit, 64-bit
Address sizes:                      48 bits physical, 48 bits virtual
Byte Order:                         Little Endian
CPU(s):                             12
On-line CPU(s) list:                0-11
Vendor ID:                          AuthenticAMD
Model name:                         AMD Ryzen 5 3600 6-Core Processor
CPU family:                         23
Model:                              113
Thread(s) per core:                 2
Core(s) per socket:                 6
Socket(s):                          1
Stepping:                           0
BogoMIPS:                           7200.03
Flags:                              fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx mmxext fxsr_opt pdpe1gb rdtscp lm constant_tsc rep_good nopl tsc_reliable nonstop_tsc cpuid extd_apicid pni pclmulqdq ssse3 fma cx16 sse4_1 sse4_2 movbe popcnt aes xsave avx f16c rdrand hypervisor lahf_lm cmp_legacy svm cr8_legacy abm sse4a misalignsse 3dnowprefetch osvw topoext perfctr_core ssbd ibpb stibp vmmcall fsgsbase bmi1 avx2 smep bmi2 rdseed adx smap clflushopt clwb sha_ni xsaveopt xsavec xgetbv1 clzero xsaveerptr arat npt nrip_save tsc_scale vmcb_clean flushbyasid decodeassists pausefilter pfthreshold v_vmsave_vmload umip rdpid
Virtualization:                     AMD-V
Hypervisor vendor:                  Microsoft
Virtualization type:                full
L1d cache:                          192 KiB (6 instances)
L1i cache:                          192 KiB (6 instances)
L2 cache:                           3 MiB (6 instances)
L3 cache:                           16 MiB (1 instance)
Vulnerability Gather data sampling: Not affected
Vulnerability Itlb multihit:        Not affected
Vulnerability L1tf:                 Not affected
Vulnerability Mds:                  Not affected
Vulnerability Meltdown:             Not affected
Vulnerability Mmio stale data:      Not affected
Vulnerability Retbleed:             Mitigation; untrained return thunk; SMT enabled with STIBP protection
Vulnerability Spec rstack overflow: Mitigation; safe RET
Vulnerability Spec store bypass:    Mitigation; Speculative Store Bypass disabled via prctl and seccomp
Vulnerability Spectre v1:           Mitigation; usercopy/swapgs barriers and __user pointer sanitization
Vulnerability Spectre v2:           Mitigation; Retpolines, IBPB conditional, STIBP always-on, RSB filling, PBRSB-eIBRS Not affected
Vulnerability Srbds:                Not affected
Vulnerability Tsx async abort:      Not affected
```
