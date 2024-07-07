1.compile with g++ cpusched.cpp -o cpusched
2.run with ./cpusched
3.if output file is not visible,you can search and open it from vscode open-file
3.ideally try to keep bt divisible by cores
4.give feasible inputs like have bt less than or equal to k
5.i am commenting cpucheckaffinity function,you can check which cpu is available for which thread number with it,need to give i as input to it.
6.name of input file is input.txt
7.below is lscpu command to get information about my cpu
Architecture:            x86_64
  CPU op-mode(s):        32-bit, 64-bit
  Address sizes:         39 bits physical, 48 bits virtual
  Byte Order:            Little Endian
CPU(s):                  8
  On-line CPU(s) list:   0-7
Vendor ID:               GenuineIntel
  Model name:            11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
    CPU family:          6
    Model:               140
    Thread(s) per core:  2
    Core(s) per socket:  4
    Socket(s):           1
    Stepping:            1
    CPU max MHz:         4700.0000
    CPU min MHz:         400.0000
    BogoMIPS:            5606.40
    Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mc
                         a cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss 
                         ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art
                          arch_perfmon pebs bts rep_good nopl xtopology nonstop_
                         tsc cpuid aperfmperf tsc_known_freq pni pclmulqdq dtes6
                         4 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr p
                         dcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline
                         _timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefe
                         tch cpuid_fault epb cat_l2 invpcid_single cdp_l2 ssbd i
                         brs ibpb stibp ibrs_enhanced tpr_shadow flexpriority ep
                         t vpid ept_ad fsgsbase tsc_adjust bmi1 avx2 smep bmi2 e
                         rms invpcid rdt_a avx512f avx512dq rdseed adx smap avx5
                         12ifma clflushopt clwb intel_pt avx512cd sha_ni avx512b
                         w avx512vl xsaveopt xsavec xgetbv1 xsaves split_lock_de
                         tect dtherm ida arat pln pts hwp hwp_notify hwp_act_win
                         dow hwp_epp hwp_pkg_req vnmi avx512vbmi umip pku ospke 
                         avx512_vbmi2 gfni vaes vpclmulqdq avx512_vnni avx512_bi
                         talg avx512_vpopcntdq rdpid movdiri movdir64b fsrm avx5
                         12_vp2intersect md_clear ibt flush_l1d arch_capabilitie
                         s
Virtualization features: 
  Virtualization:        VT-x
Caches (sum of all):     
  L1d:                   192 KiB (4 instances)
  L1i:                   128 KiB (4 instances)
  L2:                    5 MiB (4 instances)
  L3:                    12 MiB (1 instance)
NUMA:                    
  NUMA node(s):          1
  NUMA node0 CPU(s):     0-7
Vulnerabilities:         
  Gather data sampling:  Mitigation; Microcode
  Itlb multihit:         Not affected
  L1tf:                  Not affected
  Mds:                   Not affected
  Meltdown:              Not affected
  Mmio stale data:       Not affected
  Retbleed:              Not affected
  Spec rstack overflow:  Not affected
  Spec store bypass:     Mitigation; Speculative Store Bypass disabled via prctl
  Spectre v1:            Mitigation; usercopy/swapgs barriers and __user pointer
                          sanitization
  Spectre v2:            Mitigation; Enhanced / Automatic IBRS, IBPB conditional
                         , RSB filling, PBRSB-eIBRS SW sequence
  Srbds:                 Not affected
  Tsx async abort:       Not affected
