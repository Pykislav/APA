# String Initialization and To Base 10 String

Compiler : g++

Accumulated performance (nanoseconds)

### Using base 2<sup>16</sup> number system representation.

string's size use to initialize = 5500

| cases | microseconds |
| ----- | ----------- |
| base 2 initialization | 18930 |
| base 8 initialization | 20118 |
| base 10 initialization | 19492 |
| base 16 initialization | 98 |
| to_base10_string() | 22345 |

the to_base10_string() benchmark is from a 1375 limbs size big integer

### Using base 2<sup>32</sup> number system representation.

string's size use to initialize = 5500

| cases | microseconds |
| ----- | ----------- |
| base 2 initialization | 17261 |
| base 8 initialization | 17604 |
| base 10 initialization | 17152 |
| base 16 initialization | 70 |
| to_base10_string() | 14662 |

the to_base10_string() benchmark is from a 688 limbs size big integer

### Using base 2<sup>64</sup> number system representation.

string's size use to initialize = 5500

| cases | microseconds |
| ----- | ----------- |
| base 2 initialization | 17922 |
| base 8 initialization | 17250 |
| base 10 initialization | 19585 |
| base 16 initialization | 74 |
| to_base10_string() | 12408 |

the to_base10_string() benchmark is from a 344 limbs size big integer


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
