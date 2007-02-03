#if 0
//
// Generated by Microsoft (R) D3DX9 Shader Compiler 9.11.519.0000
//
//   fxc /Fhd:\Projects\Fuji\Pc\Shaders\MatStandard.h
//    d:\Projects\Fuji\Pc\Shaders\MatStandard.vsh /Tvs_1_1 /Zi /nologo
//
//
// Parameters:
//
//   float4 bAnimation[2];
//   float4 gColourMask;
//   float4 mAnimMats[210];
//   float4 mTexMatrix[2];
//   float4x4 mWorldToScreen;
//
//
// Registers:
//
//   Name           Reg   Size
//   -------------- ----- ----
//   mWorldToScreen c0       4
//   mTexMatrix     c8       2
//   gColourMask    c10      1
//   bAnimation     c11      2
//   mAnimMats      c20    210
//

    vs_1_1
    def c4, 255, 0.5, 0, 0
    dcl_position v0  // input<0,1,2,3>
    dcl_texcoord v1  // input<7,8,9,10>
    dcl_color v2  // input<11,12,13,14>
    dcl_blendweight v3  // input<15,16,17,18>
    dcl_blendindices v4  // input<19,20,21,22>

#line 55 "d:\Projects\Fuji\Pc\Shaders\MatStandard.vsh"
    mad r0, v4, c4.x, c4.y  // input<19,20,21,22>

#line 69
    frc r3.xy, r0.zwzw
    mov r1.zw, r3.xyxy
    frc r1.xy, r0
    add r2, r0, -r1
    slt r1, -r1, r1
    slt r0, r0, -r0
    mad r0, r0, r1, r2  // i<0,0,0,0>

#line 62
    mov a0.x, r0.y
    dp4 r1.x, v0, c20[a0.x]  // t1<0>
    dp4 r1.y, v0, c21[a0.x]  // t1<1>
    dp4 r1.z, v0, c22[a0.x]  // t1<2>

#line 74
    mul r2.xyz, r1, v3.y

#line 58
    mov a0.x, r0.x
    dp4 r1.x, v0, c20[a0.x]  // t0<0>
    dp4 r1.y, v0, c21[a0.x]  // t0<1>
    dp4 r1.z, v0, c22[a0.x]  // t0<2>

#line 74
    mad r1.xyz, r1, v3.x, r2

#line 66
    mov a0.x, r0.z
    dp4 r0.x, v0, c20[a0.x]  // t2<0>
    dp4 r0.y, v0, c21[a0.x]  // t2<1>
    dp4 r0.z, v0, c22[a0.x]  // t2<2>

#line 74
    mad r1.xyz, r0, v3.z, r1

#line 70
    mov a0.x, r0.w
    dp4 r0.x, v0, c20[a0.x]  // t3<0>
    dp4 r0.y, v0, c21[a0.x]  // t3<1>
    dp4 r0.z, v0, c22[a0.x]  // t3<2>
    mad r1.xyz, r0, v3.w, r1

#line 36
    mul r0, v0, c11  // pos<0,1,2,3>

#line 74
    mad r0.xyz, r1, c12, r0  // pos<0,1,2>

#line 78
    dp4 oPos.x, r0, c0  // output<0>
    dp4 oPos.y, r0, c1  // output<1>
    dp4 oPos.z, r0, c2  // output<2>
    dp4 oPos.w, r0, c3  // output<3>

#line 81
    dp4 r0.x, v1, c8  // output<8>
    dp4 r0.y, v1, c9  // output<9>

#line 86
    mad oD0, v2, c10.xxxz, c10.yyyw  // output<4,5,6,7>

#line 81
    mov oT0, r0.xyyy  // ::main<8,9,10,11>

// approximately 42 instruction slots used
#endif

const DWORD g_vs11_main[] =
{
    0xfffe0101, 0x013afffe, 0x47554244, 0x00000028, 0x000004b0, 0x00000000, 
    0x00000001, 0x00000054, 0x0000002c, 0x00000058, 0x00000009, 0x000003fc, 
    0x000001b8, 0x505c3a64, 0x656a6f72, 0x5c737463, 0x696a7546, 0x5c63505c, 
    0x64616853, 0x5c737265, 0x5374614d, 0x646e6174, 0x2e647261, 0x00687376, 
    0x00000028, 0xffff0000, 0x00000634, 0xffff0000, 0x0000064c, 0xffff0000, 
    0x00000658, 0xffff0000, 0x00000664, 0xffff0000, 0x00000670, 0xffff0000, 
    0x0000067c, 0x00000037, 0x00000688, 0x00000045, 0x0000069c, 0x00000045, 
    0x000006a8, 0x00000045, 0x000006b4, 0x00000045, 0x000006c0, 0x00000045, 
    0x000006d0, 0x00000045, 0x000006e0, 0x00000045, 0x000006f0, 0x0000003e, 
    0x00000704, 0x0000003e, 0x00000710, 0x0000003f, 0x00000720, 0x00000040, 
    0x00000730, 0x0000004a, 0x00000740, 0x0000003a, 0x00000750, 0x0000003a, 
    0x0000075c, 0x0000003b, 0x0000076c, 0x0000003c, 0x0000077c, 0x0000004a, 
    0x0000078c, 0x00000042, 0x000007a0, 0x00000042, 0x000007ac, 0x00000043, 
    0x000007bc, 0x00000044, 0x000007cc, 0x0000004a, 0x000007dc, 0x00000046, 
    0x000007f0, 0x00000046, 0x000007fc, 0x00000047, 0x0000080c, 0x00000048, 
    0x0000081c, 0x0000004a, 0x0000082c, 0x00000024, 0x00000840, 0x0000004a, 
    0x00000850, 0x0000004e, 0x00000864, 0x0000004e, 0x00000874, 0x0000004e, 
    0x00000884, 0x0000004e, 0x00000894, 0x00000051, 0x000008a4, 0x00000052, 
    0x000008b4, 0x00000056, 0x000008c4, 0x00000051, 0x000008d8, 0x6e69616d, 
    0x736f7000, 0xababab00, 0x00030001, 0x00040001, 0x00000001, 0x00000000, 
    0x6f6c6f63, 0x75007275, 0xabab0076, 0x000001bd, 0x000001c4, 0x000001d4, 
    0x000001c4, 0x000001db, 0x000001c4, 0x00000005, 0x000c0001, 0x00030001, 
    0x000001e0, 0x0000002b, 0x00090008, 0x000b000a, 0xabab0069, 0x00020000, 
    0x00010001, 0x00000001, 0x00000000, 0x0000000d, 0x00000000, 0x00000000, 
    0x75706e69, 0x6f6e0074, 0xab006d72, 0x00030001, 0x00030001, 0x00000001, 
    0x00000000, 0x67696577, 0x00737468, 0x69646e69, 0x00736563, 0x000001bd, 
    0x000001c4, 0x0000023a, 0x00000240, 0x000001db, 0x000001c4, 0x000001d4, 
    0x000001c4, 0x00000250, 0x000001c4, 0x00000258, 0x000001c4, 0x00000005, 
    0x00170001, 0x00060001, 0x00000260, 0x00000001, 0x00010000, 0x00030002, 
    0x00000002, 0x00080007, 0x000a0009, 0x00000003, 0x000c000b, 0x000e000d, 
    0x00000004, 0x0010000f, 0x00120011, 0x00000005, 0x00140013, 0x00160015, 
    0x00000006, 0x00140013, 0x00160015, 0x7074756f, 0xab007475, 0x00000024, 
    0xffff0000, 0xffffffff, 0x00000025, 0x0001ffff, 0xffffffff, 0x00000026, 
    0xffffffff, 0xffff0002, 0x00000027, 0xffffffff, 0x0003ffff, 0x00000028, 
    0xffff0008, 0xffffffff, 0x00000029, 0x0009ffff, 0xffffffff, 0x0000002a, 
    0x00050004, 0x00070006, 0x00000022, 0x00010000, 0x00030002, 0x00000023, 
    0x00010000, 0xffff0002, 0xab003074, 0x00000014, 0xffff0000, 0xffffffff, 
    0x00000015, 0x0001ffff, 0xffffffff, 0x00000016, 0xffffffff, 0xffff0002, 
    0xab003174, 0x0000000f, 0xffff0000, 0xffffffff, 0x00000010, 0x0001ffff, 
    0xffffffff, 0x00000011, 0xffffffff, 0xffff0002, 0xab003274, 0x00000019, 
    0xffff0000, 0xffffffff, 0x0000001a, 0x0001ffff, 0xffffffff, 0x0000001b, 
    0xffffffff, 0xffff0002, 0xab003374, 0x0000001e, 0xffff0000, 0xffffffff, 
    0x0000001f, 0x0001ffff, 0xffffffff, 0x00000020, 0xffffffff, 0xffff0002, 
    0x00000000, 0x000001b8, 0x000001f8, 0x00000001, 0x00000208, 0x000001b8, 
    0x00000214, 0x00000218, 0x00000001, 0x00000228, 0x000001b8, 0x00000234, 
    0x00000290, 0x00000006, 0x000002a0, 0x000001b8, 0x000002e8, 0x000001f8, 
    0x00000007, 0x000002f0, 0x000001b8, 0x000001bd, 0x000001c4, 0x00000002, 
    0x00000344, 0x000001b8, 0x0000035c, 0x00000240, 0x00000003, 0x00000360, 
    0x000001b8, 0x00000384, 0x00000240, 0x00000003, 0x00000388, 0x000001b8, 
    0x000003ac, 0x00000240, 0x00000003, 0x000003b0, 0x000001b8, 0x000003d4, 
    0x00000240, 0x00000003, 0x000003d8, 0x7263694d, 0x666f736f, 0x52282074, 
    0x33442029, 0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 
    0x31312e39, 0x3931352e, 0x3030302e, 0xabab0030, 0x0050fffe, 0x42415443, 
    0x0000001c, 0x00000107, 0xfffe0101, 0x00000005, 0x0000001c, 0x00000101, 
    0x00000100, 0x00000080, 0x000b0002, 0x00020002, 0x0000008c, 0x00000000, 
    0x0000009c, 0x000a0002, 0x00020001, 0x000000a8, 0x00000000, 0x000000b8, 
    0x00140002, 0x000200d2, 0x000000c4, 0x00000000, 0x000000d4, 0x00080002, 
    0x00020002, 0x0000008c, 0x00000000, 0x000000df, 0x00000002, 0x00020004, 
    0x000000f0, 0x00000000, 0x696e4162, 0x6974616d, 0xab006e6f, 0x00030001, 
    0x00040001, 0x00000002, 0x00000000, 0x6c6f4367, 0x4d72756f, 0x006b7361, 
    0x00030001, 0x00040001, 0x00000001, 0x00000000, 0x696e416d, 0x74614d6d, 
    0xabab0073, 0x00030001, 0x00040001, 0x000000d2, 0x00000000, 0x7865546d, 
    0x7274614d, 0x6d007869, 0x6c726f57, 0x536f5464, 0x65657263, 0xabab006e, 
    0x00030003, 0x00040004, 0x00000001, 0x00000000, 0x315f7376, 0x4d00315f, 
    0x6f726369, 0x74666f73, 0x29522820, 0x44334420, 0x53203958, 0x65646168, 
    0x6f432072, 0x6c69706d, 0x39207265, 0x2e31312e, 0x2e393135, 0x30303030, 
    0xababab00, 0x00000051, 0xa00f0004, 0x437f0000, 0x3f000000, 0x00000000, 
    0x00000000, 0x0000001f, 0x80000000, 0x900f0000, 0x0000001f, 0x80000005, 
    0x900f0001, 0x0000001f, 0x8000000a, 0x900f0002, 0x0000001f, 0x80000001, 
    0x900f0003, 0x0000001f, 0x80000002, 0x900f0004, 0x00000004, 0x800f0000, 
    0x90e40004, 0xa0000004, 0xa0550004, 0x00000013, 0x80030003, 0x80ee0000, 
    0x00000001, 0x800c0001, 0x80440003, 0x00000013, 0x80030001, 0x80e40000, 
    0x00000002, 0x800f0002, 0x80e40000, 0x81e40001, 0x0000000c, 0x800f0001, 
    0x81e40001, 0x80e40001, 0x0000000c, 0x800f0000, 0x80e40000, 0x81e40000, 
    0x00000004, 0x800f0000, 0x80e40000, 0x80e40001, 0x80e40002, 0x00000001, 
    0xb0010000, 0x80550000, 0x00000009, 0x80010001, 0x90e40000, 0xa0e42014, 
    0x00000009, 0x80020001, 0x90e40000, 0xa0e42015, 0x00000009, 0x80040001, 
    0x90e40000, 0xa0e42016, 0x00000005, 0x80070002, 0x80e40001, 0x90550003, 
    0x00000001, 0xb0010000, 0x80000000, 0x00000009, 0x80010001, 0x90e40000, 
    0xa0e42014, 0x00000009, 0x80020001, 0x90e40000, 0xa0e42015, 0x00000009, 
    0x80040001, 0x90e40000, 0xa0e42016, 0x00000004, 0x80070001, 0x80e40001, 
    0x90000003, 0x80e40002, 0x00000001, 0xb0010000, 0x80aa0000, 0x00000009, 
    0x80010000, 0x90e40000, 0xa0e42014, 0x00000009, 0x80020000, 0x90e40000, 
    0xa0e42015, 0x00000009, 0x80040000, 0x90e40000, 0xa0e42016, 0x00000004, 
    0x80070001, 0x80e40000, 0x90aa0003, 0x80e40001, 0x00000001, 0xb0010000, 
    0x80ff0000, 0x00000009, 0x80010000, 0x90e40000, 0xa0e42014, 0x00000009, 
    0x80020000, 0x90e40000, 0xa0e42015, 0x00000009, 0x80040000, 0x90e40000, 
    0xa0e42016, 0x00000004, 0x80070001, 0x80e40000, 0x90ff0003, 0x80e40001, 
    0x00000005, 0x800f0000, 0x90e40000, 0xa0e4000b, 0x00000004, 0x80070000, 
    0x80e40001, 0xa0e4000c, 0x80e40000, 0x00000009, 0xc0010000, 0x80e40000, 
    0xa0e40000, 0x00000009, 0xc0020000, 0x80e40000, 0xa0e40001, 0x00000009, 
    0xc0040000, 0x80e40000, 0xa0e40002, 0x00000009, 0xc0080000, 0x80e40000, 
    0xa0e40003, 0x00000009, 0x80010000, 0x90e40001, 0xa0e40008, 0x00000009, 
    0x80020000, 0x90e40001, 0xa0e40009, 0x00000004, 0xd00f0000, 0x90e40002, 
    0xa080000a, 0xa0d5000a, 0x00000001, 0xe00f0000, 0x80540000, 0x0000ffff
};
