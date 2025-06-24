# Karatsuba Multiplication

Compiler : g++

Average performance of APA's karatsuba implementation (microseconds)

### Using base 2<sup>16</sup> number system representation.

| cases | limb dimension | microseconds |
| ----- | ----------- | -------------------- |
| case 1 | 2054x2038 | 780 μs |
| case 2 | 2054x918 | 508 μs |
| case 3 | 128x2038 | 161 μs |

### Using base 2<sup>32</sup> number system representation.

| cases | limb dimension | microseconds |
| ----- | ----------- | -------------------- |
| case 1 | 1027x1019 | 262 μs |
| case 2 | 1027x459 | 171 μs |
| case 3 | 64x1019 | 55 μs |

### Using base 2<sup>64</sup> number system representation.

| cases | limb dimension | microseconds |
| ----- | ----------- | -------------------- |
| case 1 | 514x510 | 83 μs |
| case 2 | 514x230 | 55 μs |
| case 3 | 32x510 | 11 μs |


### System Runner

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
