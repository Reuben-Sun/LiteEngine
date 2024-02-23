# SPV

构建项目CompileShader，即可生成SPV

```bash
dxc -E MainVS -spirv -Zi -Qembed_debug -O0 -T vs_6_0 -Fo ${SPV_TARGET_DIR}/${FILE_NAME}.vert.spv ${SHADER_FILE}
dxc -E MainPS -spirv -Zi -Qembed_debug -O0 -T ps_6_0 -Fo ${SPV_TARGET_DIR}/${FILE_NAME}.frag.spv ${SHADER_FILE}
```

