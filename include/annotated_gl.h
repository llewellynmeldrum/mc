#ifdef glAccum
#undef glAccum
	static inline void glAccum(GLenum op, GLfloat value){
	    glad_debug_glAccum(op, value);
	}
#endif // glAccum

#ifdef glAccumxOES
#undef glAccumxOES
	static inline void glAccumxOES(GLenum op, GLfixed value){
	    glad_debug_glAccumxOES(op, value);
	}
#endif // glAccumxOES

#ifdef glActiveProgramEXT
#undef glActiveProgramEXT
	static inline void glActiveProgramEXT(GLuint program){
	    glad_debug_glActiveProgramEXT(program);
	}
#endif // glActiveProgramEXT

#ifdef glActiveShaderProgram
#undef glActiveShaderProgram
	static inline void glActiveShaderProgram(GLuint pipeline, GLuint program){
	    glad_debug_glActiveShaderProgram(pipeline, program);
	}
#endif // glActiveShaderProgram

#ifdef glActiveShaderProgramEXT
#undef glActiveShaderProgramEXT
	static inline void glActiveShaderProgramEXT(GLuint pipeline, GLuint program){
	    glad_debug_glActiveShaderProgramEXT(pipeline, program);
	}
#endif // glActiveShaderProgramEXT

#ifdef glActiveStencilFaceEXT
#undef glActiveStencilFaceEXT
	static inline void glActiveStencilFaceEXT(GLenum face){
	    glad_debug_glActiveStencilFaceEXT(face);
	}
#endif // glActiveStencilFaceEXT

#ifdef glActiveTexture
#undef glActiveTexture
	static inline void glActiveTexture(GLenum texture){
	    glad_debug_glActiveTexture(texture);
	}
#endif // glActiveTexture

#ifdef glActiveTextureARB
#undef glActiveTextureARB
	static inline void glActiveTextureARB(GLenum texture){
	    glad_debug_glActiveTextureARB(texture);
	}
#endif // glActiveTextureARB

#ifdef glActiveVaryingNV
#undef glActiveVaryingNV
	static inline void glActiveVaryingNV(GLuint program, const GLchar *name){
	    glad_debug_glActiveVaryingNV(program, name);
	}
#endif // glActiveVaryingNV

#ifdef glAddClientPointerRangeMESA
#undef glAddClientPointerRangeMESA
	static inline void glAddClientPointerRangeMESA(GLvoid *addr, GLsizeiptr size){
	    glad_debug_glAddClientPointerRangeMESA(addr, size);
	}
#endif // glAddClientPointerRangeMESA

#ifdef glAlphaFragmentOp1ATI
#undef glAlphaFragmentOp1ATI
	static inline void glAlphaFragmentOp1ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod){
	    glad_debug_glAlphaFragmentOp1ATI(op, dst, dstMod, arg1, arg1Rep, arg1Mod);
	}
#endif // glAlphaFragmentOp1ATI

#ifdef glAlphaFragmentOp2ATI
#undef glAlphaFragmentOp2ATI
	static inline void glAlphaFragmentOp2ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod){
	    glad_debug_glAlphaFragmentOp2ATI(op, dst, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod);
	}
#endif // glAlphaFragmentOp2ATI

#ifdef glAlphaFragmentOp3ATI
#undef glAlphaFragmentOp3ATI
	static inline void glAlphaFragmentOp3ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod){
	    glad_debug_glAlphaFragmentOp3ATI(op, dst, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod, arg3, arg3Rep, arg3Mod);
	}
#endif // glAlphaFragmentOp3ATI

#ifdef glAlphaFunc
#undef glAlphaFunc
	static inline void glAlphaFunc(GLenum func, GLfloat ref){
	    glad_debug_glAlphaFunc(func, ref);
	}
#endif // glAlphaFunc

#ifdef glAlphaFuncQCOM
#undef glAlphaFuncQCOM
	static inline void glAlphaFuncQCOM(GLenum func, GLclampf ref){
	    glad_debug_glAlphaFuncQCOM(func, ref);
	}
#endif // glAlphaFuncQCOM

#ifdef glAlphaFuncx
#undef glAlphaFuncx
	static inline void glAlphaFuncx(GLenum func, GLfixed ref){
	    glad_debug_glAlphaFuncx(func, ref);
	}
#endif // glAlphaFuncx

#ifdef glAlphaFuncxOES
#undef glAlphaFuncxOES
	static inline void glAlphaFuncxOES(GLenum func, GLfixed ref){
	    glad_debug_glAlphaFuncxOES(func, ref);
	}
#endif // glAlphaFuncxOES

#ifdef glAlphaToCoverageDitherControlNV
#undef glAlphaToCoverageDitherControlNV
	static inline void glAlphaToCoverageDitherControlNV(GLenum mode){
	    glad_debug_glAlphaToCoverageDitherControlNV(mode);
	}
#endif // glAlphaToCoverageDitherControlNV

#ifdef glApplyFramebufferAttachmentCMAAINTEL
#undef glApplyFramebufferAttachmentCMAAINTEL
	static inline void glApplyFramebufferAttachmentCMAAINTEL(){
	    glad_debug_glApplyFramebufferAttachmentCMAAINTEL();
	}
#endif // glApplyFramebufferAttachmentCMAAINTEL

#ifdef glApplyTextureEXT
#undef glApplyTextureEXT
	static inline void glApplyTextureEXT(GLenum mode){
	    glad_debug_glApplyTextureEXT(mode);
	}
#endif // glApplyTextureEXT

#ifdef glArrayElement
#undef glArrayElement
	static inline void glArrayElement(GLint i){
	    glad_debug_glArrayElement(i);
	}
#endif // glArrayElement

#ifdef glArrayElementEXT
#undef glArrayElementEXT
	static inline void glArrayElementEXT(GLint i){
	    glad_debug_glArrayElementEXT(i);
	}
#endif // glArrayElementEXT

#ifdef glArrayObjectATI
#undef glArrayObjectATI
	static inline void glArrayObjectATI(GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset){
	    glad_debug_glArrayObjectATI(array, size, type, stride, buffer, offset);
	}
#endif // glArrayObjectATI

#ifdef glAsyncMarkerSGIX
#undef glAsyncMarkerSGIX
	static inline void glAsyncMarkerSGIX(GLuint marker){
	    glad_debug_glAsyncMarkerSGIX(marker);
	}
#endif // glAsyncMarkerSGIX

#ifdef glAttachObjectARB
#undef glAttachObjectARB
	static inline void glAttachObjectARB(GLhandleARB containerObj, GLhandleARB obj){
	    glad_debug_glAttachObjectARB(containerObj, obj);
	}
#endif // glAttachObjectARB

#ifdef glAttachShader
#undef glAttachShader
	static inline void glAttachShader(GLuint program, GLuint shader){
	    glad_debug_glAttachShader(program, shader);
	}
#endif // glAttachShader

#ifdef glBegin
#undef glBegin
	static inline void glBegin(GLenum mode){
	    glad_debug_glBegin(mode);
	}
#endif // glBegin

#ifdef glBeginConditionalRender
#undef glBeginConditionalRender
	static inline void glBeginConditionalRender(GLuint id, GLenum mode){
	    glad_debug_glBeginConditionalRender(id, mode);
	}
#endif // glBeginConditionalRender

#ifdef glBeginConditionalRenderNV
#undef glBeginConditionalRenderNV
	static inline void glBeginConditionalRenderNV(GLuint id, GLenum mode){
	    glad_debug_glBeginConditionalRenderNV(id, mode);
	}
#endif // glBeginConditionalRenderNV

#ifdef glBeginConditionalRenderNVX
#undef glBeginConditionalRenderNVX
	static inline void glBeginConditionalRenderNVX(GLuint id){
	    glad_debug_glBeginConditionalRenderNVX(id);
	}
#endif // glBeginConditionalRenderNVX

#ifdef glBeginFragmentShaderATI
#undef glBeginFragmentShaderATI
	static inline void glBeginFragmentShaderATI(){
	    glad_debug_glBeginFragmentShaderATI();
	}
#endif // glBeginFragmentShaderATI

#ifdef glBeginOcclusionQueryNV
#undef glBeginOcclusionQueryNV
	static inline void glBeginOcclusionQueryNV(GLuint id){
	    glad_debug_glBeginOcclusionQueryNV(id);
	}
#endif // glBeginOcclusionQueryNV

#ifdef glBeginPerfMonitorAMD
#undef glBeginPerfMonitorAMD
	static inline void glBeginPerfMonitorAMD(GLuint monitor){
	    glad_debug_glBeginPerfMonitorAMD(monitor);
	}
#endif // glBeginPerfMonitorAMD

#ifdef glBeginPerfQueryINTEL
#undef glBeginPerfQueryINTEL
	static inline void glBeginPerfQueryINTEL(GLuint queryHandle){
	    glad_debug_glBeginPerfQueryINTEL(queryHandle);
	}
#endif // glBeginPerfQueryINTEL

#ifdef glBeginQuery
#undef glBeginQuery
	static inline void glBeginQuery(GLenum target, GLuint id){
	    glad_debug_glBeginQuery(target, id);
	}
#endif // glBeginQuery

#ifdef glBeginQueryARB
#undef glBeginQueryARB
	static inline void glBeginQueryARB(GLenum target, GLuint id){
	    glad_debug_glBeginQueryARB(target, id);
	}
#endif // glBeginQueryARB

#ifdef glBeginQueryEXT
#undef glBeginQueryEXT
	static inline void glBeginQueryEXT(GLenum target, GLuint id){
	    glad_debug_glBeginQueryEXT(target, id);
	}
#endif // glBeginQueryEXT

#ifdef glBeginQueryIndexed
#undef glBeginQueryIndexed
	static inline void glBeginQueryIndexed(GLenum target, GLuint index, GLuint id){
	    glad_debug_glBeginQueryIndexed(target, index, id);
	}
#endif // glBeginQueryIndexed

#ifdef glBeginTransformFeedback
#undef glBeginTransformFeedback
	static inline void glBeginTransformFeedback(GLenum primitiveMode){
	    glad_debug_glBeginTransformFeedback(primitiveMode);
	}
#endif // glBeginTransformFeedback

#ifdef glBeginTransformFeedbackEXT
#undef glBeginTransformFeedbackEXT
	static inline void glBeginTransformFeedbackEXT(GLenum primitiveMode){
	    glad_debug_glBeginTransformFeedbackEXT(primitiveMode);
	}
#endif // glBeginTransformFeedbackEXT

#ifdef glBeginTransformFeedbackNV
#undef glBeginTransformFeedbackNV
	static inline void glBeginTransformFeedbackNV(GLenum primitiveMode){
	    glad_debug_glBeginTransformFeedbackNV(primitiveMode);
	}
#endif // glBeginTransformFeedbackNV

#ifdef glBeginVertexShaderEXT
#undef glBeginVertexShaderEXT
	static inline void glBeginVertexShaderEXT(){
	    glad_debug_glBeginVertexShaderEXT();
	}
#endif // glBeginVertexShaderEXT

#ifdef glBeginVideoCaptureNV
#undef glBeginVideoCaptureNV
	static inline void glBeginVideoCaptureNV(GLuint video_capture_slot){
	    glad_debug_glBeginVideoCaptureNV(video_capture_slot);
	}
#endif // glBeginVideoCaptureNV

#ifdef glBindAttribLocation
#undef glBindAttribLocation
	static inline void glBindAttribLocation(GLuint program, GLuint index, const GLchar *name){
	    glad_debug_glBindAttribLocation(program, index, name);
	}
#endif // glBindAttribLocation

#ifdef glBindAttribLocationARB
#undef glBindAttribLocationARB
	static inline void glBindAttribLocationARB(GLhandleARB programObj, GLuint index, const GLcharARB *name){
	    glad_debug_glBindAttribLocationARB(programObj, index, name);
	}
#endif // glBindAttribLocationARB

#ifdef glBindBuffer
#undef glBindBuffer
	static inline void glBindBuffer(GLenum target, GLuint buffer){
	    glad_debug_glBindBuffer(target, buffer);
	}
#endif // glBindBuffer

#ifdef glBindBufferARB
#undef glBindBufferARB
	static inline void glBindBufferARB(GLenum target, GLuint buffer){
	    glad_debug_glBindBufferARB(target, buffer);
	}
#endif // glBindBufferARB

#ifdef glBindBufferBase
#undef glBindBufferBase
	static inline void glBindBufferBase(GLenum target, GLuint index, GLuint buffer){
	    glad_debug_glBindBufferBase(target, index, buffer);
	}
#endif // glBindBufferBase

#ifdef glBindBufferBaseEXT
#undef glBindBufferBaseEXT
	static inline void glBindBufferBaseEXT(GLenum target, GLuint index, GLuint buffer){
	    glad_debug_glBindBufferBaseEXT(target, index, buffer);
	}
#endif // glBindBufferBaseEXT

#ifdef glBindBufferBaseNV
#undef glBindBufferBaseNV
	static inline void glBindBufferBaseNV(GLenum target, GLuint index, GLuint buffer){
	    glad_debug_glBindBufferBaseNV(target, index, buffer);
	}
#endif // glBindBufferBaseNV

#ifdef glBindBufferOffsetEXT
#undef glBindBufferOffsetEXT
	static inline void glBindBufferOffsetEXT(GLenum target, GLuint index, GLuint buffer, GLintptr offset){
	    glad_debug_glBindBufferOffsetEXT(target, index, buffer, offset);
	}
#endif // glBindBufferOffsetEXT

#ifdef glBindBufferOffsetNV
#undef glBindBufferOffsetNV
	static inline void glBindBufferOffsetNV(GLenum target, GLuint index, GLuint buffer, GLintptr offset){
	    glad_debug_glBindBufferOffsetNV(target, index, buffer, offset);
	}
#endif // glBindBufferOffsetNV

#ifdef glBindBufferRange
#undef glBindBufferRange
	static inline void glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size){
	    glad_debug_glBindBufferRange(target, index, buffer, offset, size);
	}
#endif // glBindBufferRange

#ifdef glBindBufferRangeEXT
#undef glBindBufferRangeEXT
	static inline void glBindBufferRangeEXT(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size){
	    glad_debug_glBindBufferRangeEXT(target, index, buffer, offset, size);
	}
#endif // glBindBufferRangeEXT

#ifdef glBindBufferRangeNV
#undef glBindBufferRangeNV
	static inline void glBindBufferRangeNV(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size){
	    glad_debug_glBindBufferRangeNV(target, index, buffer, offset, size);
	}
#endif // glBindBufferRangeNV

#ifdef glBindBuffersBase
#undef glBindBuffersBase
	static inline void glBindBuffersBase(GLenum target, GLuint first, GLsizei count, const GLuint *buffers){
	    glad_debug_glBindBuffersBase(target, first, count, buffers);
	}
#endif // glBindBuffersBase

#ifdef glBindBuffersRange
#undef glBindBuffersRange
	static inline void glBindBuffersRange(GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes){
	    glad_debug_glBindBuffersRange(target, first, count, buffers, offsets, sizes);
	}
#endif // glBindBuffersRange

#ifdef glBindFragDataLocation
#undef glBindFragDataLocation
	static inline void glBindFragDataLocation(GLuint program, GLuint color, const GLchar *name){
	    glad_debug_glBindFragDataLocation(program, color, name);
	}
#endif // glBindFragDataLocation

#ifdef glBindFragDataLocationEXT
#undef glBindFragDataLocationEXT
	static inline void glBindFragDataLocationEXT(GLuint program, GLuint color, const GLchar *name){
	    glad_debug_glBindFragDataLocationEXT(program, color, name);
	}
#endif // glBindFragDataLocationEXT

#ifdef glBindFragDataLocationIndexed
#undef glBindFragDataLocationIndexed
	static inline void glBindFragDataLocationIndexed(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name){
	    glad_debug_glBindFragDataLocationIndexed(program, colorNumber, index, name);
	}
#endif // glBindFragDataLocationIndexed

#ifdef glBindFragDataLocationIndexedEXT
#undef glBindFragDataLocationIndexedEXT
	static inline void glBindFragDataLocationIndexedEXT(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name){
	    glad_debug_glBindFragDataLocationIndexedEXT(program, colorNumber, index, name);
	}
#endif // glBindFragDataLocationIndexedEXT

#ifdef glBindFragmentShaderATI
#undef glBindFragmentShaderATI
	static inline void glBindFragmentShaderATI(GLuint id){
	    glad_debug_glBindFragmentShaderATI(id);
	}
#endif // glBindFragmentShaderATI

#ifdef glBindFramebuffer
#undef glBindFramebuffer
	static inline void glBindFramebuffer(GLenum target, GLuint framebuffer){
	    glad_debug_glBindFramebuffer(target, framebuffer);
	}
#endif // glBindFramebuffer

#ifdef glBindFramebufferEXT
#undef glBindFramebufferEXT
	static inline void glBindFramebufferEXT(GLenum target, GLuint framebuffer){
	    glad_debug_glBindFramebufferEXT(target, framebuffer);
	}
#endif // glBindFramebufferEXT

#ifdef glBindFramebufferOES
#undef glBindFramebufferOES
	static inline void glBindFramebufferOES(GLenum target, GLuint framebuffer){
	    glad_debug_glBindFramebufferOES(target, framebuffer);
	}
#endif // glBindFramebufferOES

#ifdef glBindImageTexture
#undef glBindImageTexture
	static inline void glBindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format){
	    glad_debug_glBindImageTexture(unit, texture, level, layered, layer, access, format);
	}
#endif // glBindImageTexture

#ifdef glBindImageTextureEXT
#undef glBindImageTextureEXT
	static inline void glBindImageTextureEXT(GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format){
	    glad_debug_glBindImageTextureEXT(index, texture, level, layered, layer, access, format);
	}
#endif // glBindImageTextureEXT

#ifdef glBindImageTextures
#undef glBindImageTextures
	static inline void glBindImageTextures(GLuint first, GLsizei count, const GLuint *textures){
	    glad_debug_glBindImageTextures(first, count, textures);
	}
#endif // glBindImageTextures

#ifdef glBindMultiTextureEXT
#undef glBindMultiTextureEXT
	static inline void glBindMultiTextureEXT(GLenum texunit, GLenum target, GLuint texture){
	    glad_debug_glBindMultiTextureEXT(texunit, target, texture);
	}
#endif // glBindMultiTextureEXT

#ifdef glBindProgramARB
#undef glBindProgramARB
	static inline void glBindProgramARB(GLenum target, GLuint program){
	    glad_debug_glBindProgramARB(target, program);
	}
#endif // glBindProgramARB

#ifdef glBindProgramNV
#undef glBindProgramNV
	static inline void glBindProgramNV(GLenum target, GLuint id){
	    glad_debug_glBindProgramNV(target, id);
	}
#endif // glBindProgramNV

#ifdef glBindProgramPipeline
#undef glBindProgramPipeline
	static inline void glBindProgramPipeline(GLuint pipeline){
	    glad_debug_glBindProgramPipeline(pipeline);
	}
#endif // glBindProgramPipeline

#ifdef glBindProgramPipelineEXT
#undef glBindProgramPipelineEXT
	static inline void glBindProgramPipelineEXT(GLuint pipeline){
	    glad_debug_glBindProgramPipelineEXT(pipeline);
	}
#endif // glBindProgramPipelineEXT

#ifdef glBindRenderbuffer
#undef glBindRenderbuffer
	static inline void glBindRenderbuffer(GLenum target, GLuint renderbuffer){
	    glad_debug_glBindRenderbuffer(target, renderbuffer);
	}
#endif // glBindRenderbuffer

#ifdef glBindRenderbufferEXT
#undef glBindRenderbufferEXT
	static inline void glBindRenderbufferEXT(GLenum target, GLuint renderbuffer){
	    glad_debug_glBindRenderbufferEXT(target, renderbuffer);
	}
#endif // glBindRenderbufferEXT

#ifdef glBindRenderbufferOES
#undef glBindRenderbufferOES
	static inline void glBindRenderbufferOES(GLenum target, GLuint renderbuffer){
	    glad_debug_glBindRenderbufferOES(target, renderbuffer);
	}
#endif // glBindRenderbufferOES

#ifdef glBindSampler
#undef glBindSampler
	static inline void glBindSampler(GLuint unit, GLuint sampler){
	    glad_debug_glBindSampler(unit, sampler);
	}
#endif // glBindSampler

#ifdef glBindSamplers
#undef glBindSamplers
	static inline void glBindSamplers(GLuint first, GLsizei count, const GLuint *samplers){
	    glad_debug_glBindSamplers(first, count, samplers);
	}
#endif // glBindSamplers

#ifdef glBindShadingRateImageNV
#undef glBindShadingRateImageNV
	static inline void glBindShadingRateImageNV(GLuint texture){
	    glad_debug_glBindShadingRateImageNV(texture);
	}
#endif // glBindShadingRateImageNV

#ifdef glBindTexture
#undef glBindTexture
	static inline void glBindTexture(GLenum target, GLuint texture){
	    glad_debug_glBindTexture(target, texture);
	}
#endif // glBindTexture

#ifdef glBindTextureEXT
#undef glBindTextureEXT
	static inline void glBindTextureEXT(GLenum target, GLuint texture){
	    glad_debug_glBindTextureEXT(target, texture);
	}
#endif // glBindTextureEXT

#ifdef glBindTextureUnit
#undef glBindTextureUnit
	static inline void glBindTextureUnit(GLuint unit, GLuint texture){
	    glad_debug_glBindTextureUnit(unit, texture);
	}
#endif // glBindTextureUnit

#ifdef glBindTextures
#undef glBindTextures
	static inline void glBindTextures(GLuint first, GLsizei count, const GLuint *textures){
	    glad_debug_glBindTextures(first, count, textures);
	}
#endif // glBindTextures

#ifdef glBindTransformFeedback
#undef glBindTransformFeedback
	static inline void glBindTransformFeedback(GLenum target, GLuint id){
	    glad_debug_glBindTransformFeedback(target, id);
	}
#endif // glBindTransformFeedback

#ifdef glBindTransformFeedbackNV
#undef glBindTransformFeedbackNV
	static inline void glBindTransformFeedbackNV(GLenum target, GLuint id){
	    glad_debug_glBindTransformFeedbackNV(target, id);
	}
#endif // glBindTransformFeedbackNV

#ifdef glBindVertexArray
#undef glBindVertexArray
	static inline void glBindVertexArray(GLuint array){
	    glad_debug_glBindVertexArray(array);
	}
#endif // glBindVertexArray

#ifdef glBindVertexArrayAPPLE
#undef glBindVertexArrayAPPLE
	static inline void glBindVertexArrayAPPLE(GLuint array){
	    glad_debug_glBindVertexArrayAPPLE(array);
	}
#endif // glBindVertexArrayAPPLE

#ifdef glBindVertexArrayOES
#undef glBindVertexArrayOES
	static inline void glBindVertexArrayOES(GLuint array){
	    glad_debug_glBindVertexArrayOES(array);
	}
#endif // glBindVertexArrayOES

#ifdef glBindVertexBuffer
#undef glBindVertexBuffer
	static inline void glBindVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride){
	    glad_debug_glBindVertexBuffer(bindingindex, buffer, offset, stride);
	}
#endif // glBindVertexBuffer

#ifdef glBindVertexBuffers
#undef glBindVertexBuffers
	static inline void glBindVertexBuffers(GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides){
	    glad_debug_glBindVertexBuffers(first, count, buffers, offsets, strides);
	}
#endif // glBindVertexBuffers

#ifdef glBindVertexShaderEXT
#undef glBindVertexShaderEXT
	static inline void glBindVertexShaderEXT(GLuint id){
	    glad_debug_glBindVertexShaderEXT(id);
	}
#endif // glBindVertexShaderEXT

#ifdef glBindVideoCaptureStreamBufferNV
#undef glBindVideoCaptureStreamBufferNV
	static inline void glBindVideoCaptureStreamBufferNV(GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLintptrARB offset){
	    glad_debug_glBindVideoCaptureStreamBufferNV(video_capture_slot, stream, frame_region, offset);
	}
#endif // glBindVideoCaptureStreamBufferNV

#ifdef glBindVideoCaptureStreamTextureNV
#undef glBindVideoCaptureStreamTextureNV
	static inline void glBindVideoCaptureStreamTextureNV(GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLenum target, GLuint texture){
	    glad_debug_glBindVideoCaptureStreamTextureNV(video_capture_slot, stream, frame_region, target, texture);
	}
#endif // glBindVideoCaptureStreamTextureNV

#ifdef glBinormal3bEXT
#undef glBinormal3bEXT
	static inline void glBinormal3bEXT(GLbyte bx, GLbyte by, GLbyte bz){
	    glad_debug_glBinormal3bEXT(bx, by, bz);
	}
#endif // glBinormal3bEXT

#ifdef glBinormal3bvEXT
#undef glBinormal3bvEXT
	static inline void glBinormal3bvEXT(const GLbyte *v){
	    glad_debug_glBinormal3bvEXT(v);
	}
#endif // glBinormal3bvEXT

#ifdef glBinormal3dEXT
#undef glBinormal3dEXT
	static inline void glBinormal3dEXT(GLdouble bx, GLdouble by, GLdouble bz){
	    glad_debug_glBinormal3dEXT(bx, by, bz);
	}
#endif // glBinormal3dEXT

#ifdef glBinormal3dvEXT
#undef glBinormal3dvEXT
	static inline void glBinormal3dvEXT(const GLdouble *v){
	    glad_debug_glBinormal3dvEXT(v);
	}
#endif // glBinormal3dvEXT

#ifdef glBinormal3fEXT
#undef glBinormal3fEXT
	static inline void glBinormal3fEXT(GLfloat bx, GLfloat by, GLfloat bz){
	    glad_debug_glBinormal3fEXT(bx, by, bz);
	}
#endif // glBinormal3fEXT

#ifdef glBinormal3fvEXT
#undef glBinormal3fvEXT
	static inline void glBinormal3fvEXT(const GLfloat *v){
	    glad_debug_glBinormal3fvEXT(v);
	}
#endif // glBinormal3fvEXT

#ifdef glBinormal3iEXT
#undef glBinormal3iEXT
	static inline void glBinormal3iEXT(GLint bx, GLint by, GLint bz){
	    glad_debug_glBinormal3iEXT(bx, by, bz);
	}
#endif // glBinormal3iEXT

#ifdef glBinormal3ivEXT
#undef glBinormal3ivEXT
	static inline void glBinormal3ivEXT(const GLint *v){
	    glad_debug_glBinormal3ivEXT(v);
	}
#endif // glBinormal3ivEXT

#ifdef glBinormal3sEXT
#undef glBinormal3sEXT
	static inline void glBinormal3sEXT(GLshort bx, GLshort by, GLshort bz){
	    glad_debug_glBinormal3sEXT(bx, by, bz);
	}
#endif // glBinormal3sEXT

#ifdef glBinormal3svEXT
#undef glBinormal3svEXT
	static inline void glBinormal3svEXT(const GLshort *v){
	    glad_debug_glBinormal3svEXT(v);
	}
#endif // glBinormal3svEXT

#ifdef glBitmap
#undef glBitmap
	static inline void glBitmap(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap){
	    glad_debug_glBitmap(width, height, xorig, yorig, xmove, ymove, bitmap);
	}
#endif // glBitmap

#ifdef glBitmapxOES
#undef glBitmapxOES
	static inline void glBitmapxOES(GLsizei width, GLsizei height, GLfixed xorig, GLfixed yorig, GLfixed xmove, GLfixed ymove, const GLubyte *bitmap){
	    glad_debug_glBitmapxOES(width, height, xorig, yorig, xmove, ymove, bitmap);
	}
#endif // glBitmapxOES

#ifdef glBlendBarrier
#undef glBlendBarrier
	static inline void glBlendBarrier(){
	    glad_debug_glBlendBarrier();
	}
#endif // glBlendBarrier

#ifdef glBlendBarrierKHR
#undef glBlendBarrierKHR
	static inline void glBlendBarrierKHR(){
	    glad_debug_glBlendBarrierKHR();
	}
#endif // glBlendBarrierKHR

#ifdef glBlendBarrierNV
#undef glBlendBarrierNV
	static inline void glBlendBarrierNV(){
	    glad_debug_glBlendBarrierNV();
	}
#endif // glBlendBarrierNV

#ifdef glBlendColor
#undef glBlendColor
	static inline void glBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha){
	    glad_debug_glBlendColor(red, green, blue, alpha);
	}
#endif // glBlendColor

#ifdef glBlendColorEXT
#undef glBlendColorEXT
	static inline void glBlendColorEXT(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha){
	    glad_debug_glBlendColorEXT(red, green, blue, alpha);
	}
#endif // glBlendColorEXT

#ifdef glBlendColorxOES
#undef glBlendColorxOES
	static inline void glBlendColorxOES(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha){
	    glad_debug_glBlendColorxOES(red, green, blue, alpha);
	}
#endif // glBlendColorxOES

#ifdef glBlendEquation
#undef glBlendEquation
	static inline void glBlendEquation(GLenum mode){
	    glad_debug_glBlendEquation(mode);
	}
#endif // glBlendEquation

#ifdef glBlendEquationEXT
#undef glBlendEquationEXT
	static inline void glBlendEquationEXT(GLenum mode){
	    glad_debug_glBlendEquationEXT(mode);
	}
#endif // glBlendEquationEXT

#ifdef glBlendEquationIndexedAMD
#undef glBlendEquationIndexedAMD
	static inline void glBlendEquationIndexedAMD(GLuint buf, GLenum mode){
	    glad_debug_glBlendEquationIndexedAMD(buf, mode);
	}
#endif // glBlendEquationIndexedAMD

#ifdef glBlendEquationOES
#undef glBlendEquationOES
	static inline void glBlendEquationOES(GLenum mode){
	    glad_debug_glBlendEquationOES(mode);
	}
#endif // glBlendEquationOES

#ifdef glBlendEquationSeparate
#undef glBlendEquationSeparate
	static inline void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha){
	    glad_debug_glBlendEquationSeparate(modeRGB, modeAlpha);
	}
#endif // glBlendEquationSeparate

#ifdef glBlendEquationSeparateEXT
#undef glBlendEquationSeparateEXT
	static inline void glBlendEquationSeparateEXT(GLenum modeRGB, GLenum modeAlpha){
	    glad_debug_glBlendEquationSeparateEXT(modeRGB, modeAlpha);
	}
#endif // glBlendEquationSeparateEXT

#ifdef glBlendEquationSeparateIndexedAMD
#undef glBlendEquationSeparateIndexedAMD
	static inline void glBlendEquationSeparateIndexedAMD(GLuint buf, GLenum modeRGB, GLenum modeAlpha){
	    glad_debug_glBlendEquationSeparateIndexedAMD(buf, modeRGB, modeAlpha);
	}
#endif // glBlendEquationSeparateIndexedAMD

#ifdef glBlendEquationSeparateOES
#undef glBlendEquationSeparateOES
	static inline void glBlendEquationSeparateOES(GLenum modeRGB, GLenum modeAlpha){
	    glad_debug_glBlendEquationSeparateOES(modeRGB, modeAlpha);
	}
#endif // glBlendEquationSeparateOES

#ifdef glBlendEquationSeparatei
#undef glBlendEquationSeparatei
	static inline void glBlendEquationSeparatei(GLuint buf, GLenum modeRGB, GLenum modeAlpha){
	    glad_debug_glBlendEquationSeparatei(buf, modeRGB, modeAlpha);
	}
#endif // glBlendEquationSeparatei

#ifdef glBlendEquationSeparateiARB
#undef glBlendEquationSeparateiARB
	static inline void glBlendEquationSeparateiARB(GLuint buf, GLenum modeRGB, GLenum modeAlpha){
	    glad_debug_glBlendEquationSeparateiARB(buf, modeRGB, modeAlpha);
	}
#endif // glBlendEquationSeparateiARB

#ifdef glBlendEquationSeparateiEXT
#undef glBlendEquationSeparateiEXT
	static inline void glBlendEquationSeparateiEXT(GLuint buf, GLenum modeRGB, GLenum modeAlpha){
	    glad_debug_glBlendEquationSeparateiEXT(buf, modeRGB, modeAlpha);
	}
#endif // glBlendEquationSeparateiEXT

#ifdef glBlendEquationSeparateiOES
#undef glBlendEquationSeparateiOES
	static inline void glBlendEquationSeparateiOES(GLuint buf, GLenum modeRGB, GLenum modeAlpha){
	    glad_debug_glBlendEquationSeparateiOES(buf, modeRGB, modeAlpha);
	}
#endif // glBlendEquationSeparateiOES

#ifdef glBlendEquationi
#undef glBlendEquationi
	static inline void glBlendEquationi(GLuint buf, GLenum mode){
	    glad_debug_glBlendEquationi(buf, mode);
	}
#endif // glBlendEquationi

#ifdef glBlendEquationiARB
#undef glBlendEquationiARB
	static inline void glBlendEquationiARB(GLuint buf, GLenum mode){
	    glad_debug_glBlendEquationiARB(buf, mode);
	}
#endif // glBlendEquationiARB

#ifdef glBlendEquationiEXT
#undef glBlendEquationiEXT
	static inline void glBlendEquationiEXT(GLuint buf, GLenum mode){
	    glad_debug_glBlendEquationiEXT(buf, mode);
	}
#endif // glBlendEquationiEXT

#ifdef glBlendEquationiOES
#undef glBlendEquationiOES
	static inline void glBlendEquationiOES(GLuint buf, GLenum mode){
	    glad_debug_glBlendEquationiOES(buf, mode);
	}
#endif // glBlendEquationiOES

#ifdef glBlendFunc
#undef glBlendFunc
	static inline void glBlendFunc(GLenum sfactor, GLenum dfactor){
	    glad_debug_glBlendFunc(sfactor, dfactor);
	}
#endif // glBlendFunc

#ifdef glBlendFuncIndexedAMD
#undef glBlendFuncIndexedAMD
	static inline void glBlendFuncIndexedAMD(GLuint buf, GLenum src, GLenum dst){
	    glad_debug_glBlendFuncIndexedAMD(buf, src, dst);
	}
#endif // glBlendFuncIndexedAMD

#ifdef glBlendFuncSeparate
#undef glBlendFuncSeparate
	static inline void glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha){
	    glad_debug_glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
	}
#endif // glBlendFuncSeparate

#ifdef glBlendFuncSeparateEXT
#undef glBlendFuncSeparateEXT
	static inline void glBlendFuncSeparateEXT(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha){
	    glad_debug_glBlendFuncSeparateEXT(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
	}
#endif // glBlendFuncSeparateEXT

#ifdef glBlendFuncSeparateINGR
#undef glBlendFuncSeparateINGR
	static inline void glBlendFuncSeparateINGR(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha){
	    glad_debug_glBlendFuncSeparateINGR(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
	}
#endif // glBlendFuncSeparateINGR

#ifdef glBlendFuncSeparateIndexedAMD
#undef glBlendFuncSeparateIndexedAMD
	static inline void glBlendFuncSeparateIndexedAMD(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha){
	    glad_debug_glBlendFuncSeparateIndexedAMD(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
	}
#endif // glBlendFuncSeparateIndexedAMD

#ifdef glBlendFuncSeparateOES
#undef glBlendFuncSeparateOES
	static inline void glBlendFuncSeparateOES(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha){
	    glad_debug_glBlendFuncSeparateOES(srcRGB, dstRGB, srcAlpha, dstAlpha);
	}
#endif // glBlendFuncSeparateOES

#ifdef glBlendFuncSeparatei
#undef glBlendFuncSeparatei
	static inline void glBlendFuncSeparatei(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha){
	    glad_debug_glBlendFuncSeparatei(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
	}
#endif // glBlendFuncSeparatei

#ifdef glBlendFuncSeparateiARB
#undef glBlendFuncSeparateiARB
	static inline void glBlendFuncSeparateiARB(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha){
	    glad_debug_glBlendFuncSeparateiARB(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
	}
#endif // glBlendFuncSeparateiARB

#ifdef glBlendFuncSeparateiEXT
#undef glBlendFuncSeparateiEXT
	static inline void glBlendFuncSeparateiEXT(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha){
	    glad_debug_glBlendFuncSeparateiEXT(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
	}
#endif // glBlendFuncSeparateiEXT

#ifdef glBlendFuncSeparateiOES
#undef glBlendFuncSeparateiOES
	static inline void glBlendFuncSeparateiOES(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha){
	    glad_debug_glBlendFuncSeparateiOES(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
	}
#endif // glBlendFuncSeparateiOES

#ifdef glBlendFunci
#undef glBlendFunci
	static inline void glBlendFunci(GLuint buf, GLenum src, GLenum dst){
	    glad_debug_glBlendFunci(buf, src, dst);
	}
#endif // glBlendFunci

#ifdef glBlendFunciARB
#undef glBlendFunciARB
	static inline void glBlendFunciARB(GLuint buf, GLenum src, GLenum dst){
	    glad_debug_glBlendFunciARB(buf, src, dst);
	}
#endif // glBlendFunciARB

#ifdef glBlendFunciEXT
#undef glBlendFunciEXT
	static inline void glBlendFunciEXT(GLuint buf, GLenum src, GLenum dst){
	    glad_debug_glBlendFunciEXT(buf, src, dst);
	}
#endif // glBlendFunciEXT

#ifdef glBlendFunciOES
#undef glBlendFunciOES
	static inline void glBlendFunciOES(GLuint buf, GLenum src, GLenum dst){
	    glad_debug_glBlendFunciOES(buf, src, dst);
	}
#endif // glBlendFunciOES

#ifdef glBlendParameteriNV
#undef glBlendParameteriNV
	static inline void glBlendParameteriNV(GLenum pname, GLint value){
	    glad_debug_glBlendParameteriNV(pname, value);
	}
#endif // glBlendParameteriNV

#ifdef glBlitFramebuffer
#undef glBlitFramebuffer
	static inline void glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter){
	    glad_debug_glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	}
#endif // glBlitFramebuffer

#ifdef glBlitFramebufferANGLE
#undef glBlitFramebufferANGLE
	static inline void glBlitFramebufferANGLE(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter){
	    glad_debug_glBlitFramebufferANGLE(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	}
#endif // glBlitFramebufferANGLE

#ifdef glBlitFramebufferEXT
#undef glBlitFramebufferEXT
	static inline void glBlitFramebufferEXT(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter){
	    glad_debug_glBlitFramebufferEXT(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	}
#endif // glBlitFramebufferEXT

#ifdef glBlitFramebufferLayerEXT
#undef glBlitFramebufferLayerEXT
	static inline void glBlitFramebufferLayerEXT(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint srcLayer, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLint dstLayer, GLbitfield mask, GLenum filter){
	    glad_debug_glBlitFramebufferLayerEXT(srcX0, srcY0, srcX1, srcY1, srcLayer, dstX0, dstY0, dstX1, dstY1, dstLayer, mask, filter);
	}
#endif // glBlitFramebufferLayerEXT

#ifdef glBlitFramebufferLayersEXT
#undef glBlitFramebufferLayersEXT
	static inline void glBlitFramebufferLayersEXT(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter){
	    glad_debug_glBlitFramebufferLayersEXT(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	}
#endif // glBlitFramebufferLayersEXT

#ifdef glBlitFramebufferNV
#undef glBlitFramebufferNV
	static inline void glBlitFramebufferNV(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter){
	    glad_debug_glBlitFramebufferNV(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	}
#endif // glBlitFramebufferNV

#ifdef glBlitNamedFramebuffer
#undef glBlitNamedFramebuffer
	static inline void glBlitNamedFramebuffer(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter){
	    glad_debug_glBlitNamedFramebuffer(readFramebuffer, drawFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	}
#endif // glBlitNamedFramebuffer

#ifdef glBufferAddressRangeNV
#undef glBufferAddressRangeNV
	static inline void glBufferAddressRangeNV(GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length){
	    glad_debug_glBufferAddressRangeNV(pname, index, address, length);
	}
#endif // glBufferAddressRangeNV

#ifdef glBufferAttachMemoryNV
#undef glBufferAttachMemoryNV
	static inline void glBufferAttachMemoryNV(GLenum target, GLuint memory, GLuint64 offset){
	    glad_debug_glBufferAttachMemoryNV(target, memory, offset);
	}
#endif // glBufferAttachMemoryNV

#ifdef glBufferPageCommitmentARB
#undef glBufferPageCommitmentARB
	static inline void glBufferPageCommitmentARB(GLenum target, GLintptr offset, GLsizeiptr size, GLboolean commit){
	    glad_debug_glBufferPageCommitmentARB(target, offset, size, commit);
	}
#endif // glBufferPageCommitmentARB

#ifdef glBufferPageCommitmentMemNV
#undef glBufferPageCommitmentMemNV
	static inline void glBufferPageCommitmentMemNV(GLenum target, GLintptr offset, GLsizeiptr size, GLuint memory, GLuint64 memOffset, GLboolean commit){
	    glad_debug_glBufferPageCommitmentMemNV(target, offset, size, memory, memOffset, commit);
	}
#endif // glBufferPageCommitmentMemNV

#ifdef glBufferParameteriAPPLE
#undef glBufferParameteriAPPLE
	static inline void glBufferParameteriAPPLE(GLenum target, GLenum pname, GLint param){
	    glad_debug_glBufferParameteriAPPLE(target, pname, param);
	}
#endif // glBufferParameteriAPPLE

#ifdef glBufferStorageExternalEXT
#undef glBufferStorageExternalEXT
	static inline void glBufferStorageExternalEXT(GLenum target, GLintptr offset, GLsizeiptr size, GLeglClientBufferEXT clientBuffer, GLbitfield flags){
	    glad_debug_glBufferStorageExternalEXT(target, offset, size, clientBuffer, flags);
	}
#endif // glBufferStorageExternalEXT

#ifdef glBufferStorageMemEXT
#undef glBufferStorageMemEXT
	static inline void glBufferStorageMemEXT(GLenum target, GLsizeiptr size, GLuint memory, GLuint64 offset){
	    glad_debug_glBufferStorageMemEXT(target, size, memory, offset);
	}
#endif // glBufferStorageMemEXT

#ifdef glCallCommandListNV
#undef glCallCommandListNV
	static inline void glCallCommandListNV(GLuint list){
	    glad_debug_glCallCommandListNV(list);
	}
#endif // glCallCommandListNV

#ifdef glCallList
#undef glCallList
	static inline void glCallList(GLuint list){
	    glad_debug_glCallList(list);
	}
#endif // glCallList

#ifdef glClampColor
#undef glClampColor
	static inline void glClampColor(GLenum target, GLenum clamp){
	    glad_debug_glClampColor(target, clamp);
	}
#endif // glClampColor

#ifdef glClampColorARB
#undef glClampColorARB
	static inline void glClampColorARB(GLenum target, GLenum clamp){
	    glad_debug_glClampColorARB(target, clamp);
	}
#endif // glClampColorARB

#ifdef glClear
#undef glClear
	static inline void glClear(GLbitfield mask){
	    glad_debug_glClear(mask);
	}
#endif // glClear

#ifdef glClearAccum
#undef glClearAccum
	static inline void glClearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha){
	    glad_debug_glClearAccum(red, green, blue, alpha);
	}
#endif // glClearAccum

#ifdef glClearAccumxOES
#undef glClearAccumxOES
	static inline void glClearAccumxOES(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha){
	    glad_debug_glClearAccumxOES(red, green, blue, alpha);
	}
#endif // glClearAccumxOES

#ifdef glClearBufferfi
#undef glClearBufferfi
	static inline void glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil){
	    glad_debug_glClearBufferfi(buffer, drawbuffer, depth, stencil);
	}
#endif // glClearBufferfi

#ifdef glClearBufferfv
#undef glClearBufferfv
	static inline void glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value){
	    glad_debug_glClearBufferfv(buffer, drawbuffer, value);
	}
#endif // glClearBufferfv

#ifdef glClearBufferiv
#undef glClearBufferiv
	static inline void glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint *value){
	    glad_debug_glClearBufferiv(buffer, drawbuffer, value);
	}
#endif // glClearBufferiv

#ifdef glClearBufferuiv
#undef glClearBufferuiv
	static inline void glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint *value){
	    glad_debug_glClearBufferuiv(buffer, drawbuffer, value);
	}
#endif // glClearBufferuiv

#ifdef glClearColor
#undef glClearColor
	static inline void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha){
	    glad_debug_glClearColor(red, green, blue, alpha);
	}
#endif // glClearColor

#ifdef glClearColorIiEXT
#undef glClearColorIiEXT
	static inline void glClearColorIiEXT(GLint red, GLint green, GLint blue, GLint alpha){
	    glad_debug_glClearColorIiEXT(red, green, blue, alpha);
	}
#endif // glClearColorIiEXT

#ifdef glClearColorIuiEXT
#undef glClearColorIuiEXT
	static inline void glClearColorIuiEXT(GLuint red, GLuint green, GLuint blue, GLuint alpha){
	    glad_debug_glClearColorIuiEXT(red, green, blue, alpha);
	}
#endif // glClearColorIuiEXT

#ifdef glClearColorx
#undef glClearColorx
	static inline void glClearColorx(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha){
	    glad_debug_glClearColorx(red, green, blue, alpha);
	}
#endif // glClearColorx

#ifdef glClearColorxOES
#undef glClearColorxOES
	static inline void glClearColorxOES(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha){
	    glad_debug_glClearColorxOES(red, green, blue, alpha);
	}
#endif // glClearColorxOES

#ifdef glClearDepth
#undef glClearDepth
	static inline void glClearDepth(GLdouble depth){
	    glad_debug_glClearDepth(depth);
	}
#endif // glClearDepth

#ifdef glClearDepthdNV
#undef glClearDepthdNV
	static inline void glClearDepthdNV(GLdouble depth){
	    glad_debug_glClearDepthdNV(depth);
	}
#endif // glClearDepthdNV

#ifdef glClearDepthf
#undef glClearDepthf
	static inline void glClearDepthf(GLfloat d){
	    glad_debug_glClearDepthf(d);
	}
#endif // glClearDepthf

#ifdef glClearDepthfOES
#undef glClearDepthfOES
	static inline void glClearDepthfOES(GLclampf depth){
	    glad_debug_glClearDepthfOES(depth);
	}
#endif // glClearDepthfOES

#ifdef glClearDepthx
#undef glClearDepthx
	static inline void glClearDepthx(GLfixed depth){
	    glad_debug_glClearDepthx(depth);
	}
#endif // glClearDepthx

#ifdef glClearDepthxOES
#undef glClearDepthxOES
	static inline void glClearDepthxOES(GLfixed depth){
	    glad_debug_glClearDepthxOES(depth);
	}
#endif // glClearDepthxOES

#ifdef glClearIndex
#undef glClearIndex
	static inline void glClearIndex(GLfloat c){
	    glad_debug_glClearIndex(c);
	}
#endif // glClearIndex

#ifdef glClearNamedFramebufferfi
#undef glClearNamedFramebufferfi
	static inline void glClearNamedFramebufferfi(GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil){
	    glad_debug_glClearNamedFramebufferfi(framebuffer, buffer, drawbuffer, depth, stencil);
	}
#endif // glClearNamedFramebufferfi

#ifdef glClearNamedFramebufferfv
#undef glClearNamedFramebufferfv
	static inline void glClearNamedFramebufferfv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat *value){
	    glad_debug_glClearNamedFramebufferfv(framebuffer, buffer, drawbuffer, value);
	}
#endif // glClearNamedFramebufferfv

#ifdef glClearNamedFramebufferiv
#undef glClearNamedFramebufferiv
	static inline void glClearNamedFramebufferiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint *value){
	    glad_debug_glClearNamedFramebufferiv(framebuffer, buffer, drawbuffer, value);
	}
#endif // glClearNamedFramebufferiv

#ifdef glClearNamedFramebufferuiv
#undef glClearNamedFramebufferuiv
	static inline void glClearNamedFramebufferuiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint *value){
	    glad_debug_glClearNamedFramebufferuiv(framebuffer, buffer, drawbuffer, value);
	}
#endif // glClearNamedFramebufferuiv

#ifdef glClearPixelLocalStorageuiEXT
#undef glClearPixelLocalStorageuiEXT
	static inline void glClearPixelLocalStorageuiEXT(GLsizei offset, GLsizei n, const GLuint *values){
	    glad_debug_glClearPixelLocalStorageuiEXT(offset, n, values);
	}
#endif // glClearPixelLocalStorageuiEXT

#ifdef glClearStencil
#undef glClearStencil
	static inline void glClearStencil(GLint s){
	    glad_debug_glClearStencil(s);
	}
#endif // glClearStencil

#ifdef glClientActiveTexture
#undef glClientActiveTexture
	static inline void glClientActiveTexture(GLenum texture){
	    glad_debug_glClientActiveTexture(texture);
	}
#endif // glClientActiveTexture

#ifdef glClientActiveTextureARB
#undef glClientActiveTextureARB
	static inline void glClientActiveTextureARB(GLenum texture){
	    glad_debug_glClientActiveTextureARB(texture);
	}
#endif // glClientActiveTextureARB

#ifdef glClientActiveVertexStreamATI
#undef glClientActiveVertexStreamATI
	static inline void glClientActiveVertexStreamATI(GLenum stream){
	    glad_debug_glClientActiveVertexStreamATI(stream);
	}
#endif // glClientActiveVertexStreamATI

#ifdef glClientAttribDefaultEXT
#undef glClientAttribDefaultEXT
	static inline void glClientAttribDefaultEXT(GLbitfield mask){
	    glad_debug_glClientAttribDefaultEXT(mask);
	}
#endif // glClientAttribDefaultEXT

#ifdef glClientWaitSemaphoreui64NVX
#undef glClientWaitSemaphoreui64NVX
	static inline void glClientWaitSemaphoreui64NVX(GLsizei fenceObjectCount, const GLuint *semaphoreArray, const GLuint64 *fenceValueArray){
	    glad_debug_glClientWaitSemaphoreui64NVX(fenceObjectCount, semaphoreArray, fenceValueArray);
	}
#endif // glClientWaitSemaphoreui64NVX

#ifdef glClipControl
#undef glClipControl
	static inline void glClipControl(GLenum origin, GLenum depth){
	    glad_debug_glClipControl(origin, depth);
	}
#endif // glClipControl

#ifdef glClipControlEXT
#undef glClipControlEXT
	static inline void glClipControlEXT(GLenum origin, GLenum depth){
	    glad_debug_glClipControlEXT(origin, depth);
	}
#endif // glClipControlEXT

#ifdef glClipPlane
#undef glClipPlane
	static inline void glClipPlane(GLenum plane, const GLdouble *equation){
	    glad_debug_glClipPlane(plane, equation);
	}
#endif // glClipPlane

#ifdef glClipPlanef
#undef glClipPlanef
	static inline void glClipPlanef(GLenum p, const GLfloat *eqn){
	    glad_debug_glClipPlanef(p, eqn);
	}
#endif // glClipPlanef

#ifdef glClipPlanefIMG
#undef glClipPlanefIMG
	static inline void glClipPlanefIMG(GLenum p, const GLfloat *eqn){
	    glad_debug_glClipPlanefIMG(p, eqn);
	}
#endif // glClipPlanefIMG

#ifdef glClipPlanefOES
#undef glClipPlanefOES
	static inline void glClipPlanefOES(GLenum plane, const GLfloat *equation){
	    glad_debug_glClipPlanefOES(plane, equation);
	}
#endif // glClipPlanefOES

#ifdef glClipPlanex
#undef glClipPlanex
	static inline void glClipPlanex(GLenum plane, const GLfixed *equation){
	    glad_debug_glClipPlanex(plane, equation);
	}
#endif // glClipPlanex

#ifdef glClipPlanexIMG
#undef glClipPlanexIMG
	static inline void glClipPlanexIMG(GLenum p, const GLfixed *eqn){
	    glad_debug_glClipPlanexIMG(p, eqn);
	}
#endif // glClipPlanexIMG

#ifdef glClipPlanexOES
#undef glClipPlanexOES
	static inline void glClipPlanexOES(GLenum plane, const GLfixed *equation){
	    glad_debug_glClipPlanexOES(plane, equation);
	}
#endif // glClipPlanexOES

#ifdef glColor3b
#undef glColor3b
	static inline void glColor3b(GLbyte red, GLbyte green, GLbyte blue){
	    glad_debug_glColor3b(red, green, blue);
	}
#endif // glColor3b

#ifdef glColor3bv
#undef glColor3bv
	static inline void glColor3bv(const GLbyte *v){
	    glad_debug_glColor3bv(v);
	}
#endif // glColor3bv

#ifdef glColor3d
#undef glColor3d
	static inline void glColor3d(GLdouble red, GLdouble green, GLdouble blue){
	    glad_debug_glColor3d(red, green, blue);
	}
#endif // glColor3d

#ifdef glColor3dv
#undef glColor3dv
	static inline void glColor3dv(const GLdouble *v){
	    glad_debug_glColor3dv(v);
	}
#endif // glColor3dv

#ifdef glColor3f
#undef glColor3f
	static inline void glColor3f(GLfloat red, GLfloat green, GLfloat blue){
	    glad_debug_glColor3f(red, green, blue);
	}
#endif // glColor3f

#ifdef glColor3fVertex3fSUN
#undef glColor3fVertex3fSUN
	static inline void glColor3fVertex3fSUN(GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glColor3fVertex3fSUN(r, g, b, x, y, z);
	}
#endif // glColor3fVertex3fSUN

#ifdef glColor3fVertex3fvSUN
#undef glColor3fVertex3fvSUN
	static inline void glColor3fVertex3fvSUN(const GLfloat *c, const GLfloat *v){
	    glad_debug_glColor3fVertex3fvSUN(c, v);
	}
#endif // glColor3fVertex3fvSUN

#ifdef glColor3fv
#undef glColor3fv
	static inline void glColor3fv(const GLfloat *v){
	    glad_debug_glColor3fv(v);
	}
#endif // glColor3fv

#ifdef glColor3hNV
#undef glColor3hNV
	static inline void glColor3hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue){
	    glad_debug_glColor3hNV(red, green, blue);
	}
#endif // glColor3hNV

#ifdef glColor3hvNV
#undef glColor3hvNV
	static inline void glColor3hvNV(const GLhalfNV *v){
	    glad_debug_glColor3hvNV(v);
	}
#endif // glColor3hvNV

#ifdef glColor3i
#undef glColor3i
	static inline void glColor3i(GLint red, GLint green, GLint blue){
	    glad_debug_glColor3i(red, green, blue);
	}
#endif // glColor3i

#ifdef glColor3iv
#undef glColor3iv
	static inline void glColor3iv(const GLint *v){
	    glad_debug_glColor3iv(v);
	}
#endif // glColor3iv

#ifdef glColor3s
#undef glColor3s
	static inline void glColor3s(GLshort red, GLshort green, GLshort blue){
	    glad_debug_glColor3s(red, green, blue);
	}
#endif // glColor3s

#ifdef glColor3sv
#undef glColor3sv
	static inline void glColor3sv(const GLshort *v){
	    glad_debug_glColor3sv(v);
	}
#endif // glColor3sv

#ifdef glColor3ub
#undef glColor3ub
	static inline void glColor3ub(GLubyte red, GLubyte green, GLubyte blue){
	    glad_debug_glColor3ub(red, green, blue);
	}
#endif // glColor3ub

#ifdef glColor3ubv
#undef glColor3ubv
	static inline void glColor3ubv(const GLubyte *v){
	    glad_debug_glColor3ubv(v);
	}
#endif // glColor3ubv

#ifdef glColor3ui
#undef glColor3ui
	static inline void glColor3ui(GLuint red, GLuint green, GLuint blue){
	    glad_debug_glColor3ui(red, green, blue);
	}
#endif // glColor3ui

#ifdef glColor3uiv
#undef glColor3uiv
	static inline void glColor3uiv(const GLuint *v){
	    glad_debug_glColor3uiv(v);
	}
#endif // glColor3uiv

#ifdef glColor3us
#undef glColor3us
	static inline void glColor3us(GLushort red, GLushort green, GLushort blue){
	    glad_debug_glColor3us(red, green, blue);
	}
#endif // glColor3us

#ifdef glColor3usv
#undef glColor3usv
	static inline void glColor3usv(const GLushort *v){
	    glad_debug_glColor3usv(v);
	}
#endif // glColor3usv

#ifdef glColor3xOES
#undef glColor3xOES
	static inline void glColor3xOES(GLfixed red, GLfixed green, GLfixed blue){
	    glad_debug_glColor3xOES(red, green, blue);
	}
#endif // glColor3xOES

#ifdef glColor3xvOES
#undef glColor3xvOES
	static inline void glColor3xvOES(const GLfixed *components){
	    glad_debug_glColor3xvOES(components);
	}
#endif // glColor3xvOES

#ifdef glColor4b
#undef glColor4b
	static inline void glColor4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha){
	    glad_debug_glColor4b(red, green, blue, alpha);
	}
#endif // glColor4b

#ifdef glColor4bv
#undef glColor4bv
	static inline void glColor4bv(const GLbyte *v){
	    glad_debug_glColor4bv(v);
	}
#endif // glColor4bv

#ifdef glColor4d
#undef glColor4d
	static inline void glColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha){
	    glad_debug_glColor4d(red, green, blue, alpha);
	}
#endif // glColor4d

#ifdef glColor4dv
#undef glColor4dv
	static inline void glColor4dv(const GLdouble *v){
	    glad_debug_glColor4dv(v);
	}
#endif // glColor4dv

#ifdef glColor4f
#undef glColor4f
	static inline void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha){
	    glad_debug_glColor4f(red, green, blue, alpha);
	}
#endif // glColor4f

#ifdef glColor4fNormal3fVertex3fSUN
#undef glColor4fNormal3fVertex3fSUN
	static inline void glColor4fNormal3fVertex3fSUN(GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glColor4fNormal3fVertex3fSUN(r, g, b, a, nx, ny, nz, x, y, z);
	}
#endif // glColor4fNormal3fVertex3fSUN

#ifdef glColor4fNormal3fVertex3fvSUN
#undef glColor4fNormal3fVertex3fvSUN
	static inline void glColor4fNormal3fVertex3fvSUN(const GLfloat *c, const GLfloat *n, const GLfloat *v){
	    glad_debug_glColor4fNormal3fVertex3fvSUN(c, n, v);
	}
#endif // glColor4fNormal3fVertex3fvSUN

#ifdef glColor4fv
#undef glColor4fv
	static inline void glColor4fv(const GLfloat *v){
	    glad_debug_glColor4fv(v);
	}
#endif // glColor4fv

#ifdef glColor4hNV
#undef glColor4hNV
	static inline void glColor4hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha){
	    glad_debug_glColor4hNV(red, green, blue, alpha);
	}
#endif // glColor4hNV

#ifdef glColor4hvNV
#undef glColor4hvNV
	static inline void glColor4hvNV(const GLhalfNV *v){
	    glad_debug_glColor4hvNV(v);
	}
#endif // glColor4hvNV

#ifdef glColor4i
#undef glColor4i
	static inline void glColor4i(GLint red, GLint green, GLint blue, GLint alpha){
	    glad_debug_glColor4i(red, green, blue, alpha);
	}
#endif // glColor4i

#ifdef glColor4iv
#undef glColor4iv
	static inline void glColor4iv(const GLint *v){
	    glad_debug_glColor4iv(v);
	}
#endif // glColor4iv

#ifdef glColor4s
#undef glColor4s
	static inline void glColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha){
	    glad_debug_glColor4s(red, green, blue, alpha);
	}
#endif // glColor4s

#ifdef glColor4sv
#undef glColor4sv
	static inline void glColor4sv(const GLshort *v){
	    glad_debug_glColor4sv(v);
	}
#endif // glColor4sv

#ifdef glColor4ub
#undef glColor4ub
	static inline void glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha){
	    glad_debug_glColor4ub(red, green, blue, alpha);
	}
#endif // glColor4ub

#ifdef glColor4ubVertex2fSUN
#undef glColor4ubVertex2fSUN
	static inline void glColor4ubVertex2fSUN(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y){
	    glad_debug_glColor4ubVertex2fSUN(r, g, b, a, x, y);
	}
#endif // glColor4ubVertex2fSUN

#ifdef glColor4ubVertex2fvSUN
#undef glColor4ubVertex2fvSUN
	static inline void glColor4ubVertex2fvSUN(const GLubyte *c, const GLfloat *v){
	    glad_debug_glColor4ubVertex2fvSUN(c, v);
	}
#endif // glColor4ubVertex2fvSUN

#ifdef glColor4ubVertex3fSUN
#undef glColor4ubVertex3fSUN
	static inline void glColor4ubVertex3fSUN(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glColor4ubVertex3fSUN(r, g, b, a, x, y, z);
	}
#endif // glColor4ubVertex3fSUN

#ifdef glColor4ubVertex3fvSUN
#undef glColor4ubVertex3fvSUN
	static inline void glColor4ubVertex3fvSUN(const GLubyte *c, const GLfloat *v){
	    glad_debug_glColor4ubVertex3fvSUN(c, v);
	}
#endif // glColor4ubVertex3fvSUN

#ifdef glColor4ubv
#undef glColor4ubv
	static inline void glColor4ubv(const GLubyte *v){
	    glad_debug_glColor4ubv(v);
	}
#endif // glColor4ubv

#ifdef glColor4ui
#undef glColor4ui
	static inline void glColor4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha){
	    glad_debug_glColor4ui(red, green, blue, alpha);
	}
#endif // glColor4ui

#ifdef glColor4uiv
#undef glColor4uiv
	static inline void glColor4uiv(const GLuint *v){
	    glad_debug_glColor4uiv(v);
	}
#endif // glColor4uiv

#ifdef glColor4us
#undef glColor4us
	static inline void glColor4us(GLushort red, GLushort green, GLushort blue, GLushort alpha){
	    glad_debug_glColor4us(red, green, blue, alpha);
	}
#endif // glColor4us

#ifdef glColor4usv
#undef glColor4usv
	static inline void glColor4usv(const GLushort *v){
	    glad_debug_glColor4usv(v);
	}
#endif // glColor4usv

#ifdef glColor4x
#undef glColor4x
	static inline void glColor4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha){
	    glad_debug_glColor4x(red, green, blue, alpha);
	}
#endif // glColor4x

#ifdef glColor4xOES
#undef glColor4xOES
	static inline void glColor4xOES(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha){
	    glad_debug_glColor4xOES(red, green, blue, alpha);
	}
#endif // glColor4xOES

#ifdef glColor4xvOES
#undef glColor4xvOES
	static inline void glColor4xvOES(const GLfixed *components){
	    glad_debug_glColor4xvOES(components);
	}
#endif // glColor4xvOES

#ifdef glColorFormatNV
#undef glColorFormatNV
	static inline void glColorFormatNV(GLint size, GLenum type, GLsizei stride){
	    glad_debug_glColorFormatNV(size, type, stride);
	}
#endif // glColorFormatNV

#ifdef glColorFragmentOp1ATI
#undef glColorFragmentOp1ATI
	static inline void glColorFragmentOp1ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod){
	    glad_debug_glColorFragmentOp1ATI(op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod);
	}
#endif // glColorFragmentOp1ATI

#ifdef glColorFragmentOp2ATI
#undef glColorFragmentOp2ATI
	static inline void glColorFragmentOp2ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod){
	    glad_debug_glColorFragmentOp2ATI(op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod);
	}
#endif // glColorFragmentOp2ATI

#ifdef glColorFragmentOp3ATI
#undef glColorFragmentOp3ATI
	static inline void glColorFragmentOp3ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod){
	    glad_debug_glColorFragmentOp3ATI(op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod, arg3, arg3Rep, arg3Mod);
	}
#endif // glColorFragmentOp3ATI

#ifdef glColorMask
#undef glColorMask
	static inline void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha){
	    glad_debug_glColorMask(red, green, blue, alpha);
	}
#endif // glColorMask

#ifdef glColorMaskIndexedEXT
#undef glColorMaskIndexedEXT
	static inline void glColorMaskIndexedEXT(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a){
	    glad_debug_glColorMaskIndexedEXT(index, r, g, b, a);
	}
#endif // glColorMaskIndexedEXT

#ifdef glColorMaski
#undef glColorMaski
	static inline void glColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a){
	    glad_debug_glColorMaski(index, r, g, b, a);
	}
#endif // glColorMaski

#ifdef glColorMaskiEXT
#undef glColorMaskiEXT
	static inline void glColorMaskiEXT(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a){
	    glad_debug_glColorMaskiEXT(index, r, g, b, a);
	}
#endif // glColorMaskiEXT

#ifdef glColorMaskiOES
#undef glColorMaskiOES
	static inline void glColorMaskiOES(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a){
	    glad_debug_glColorMaskiOES(index, r, g, b, a);
	}
#endif // glColorMaskiOES

#ifdef glColorMaterial
#undef glColorMaterial
	static inline void glColorMaterial(GLenum face, GLenum mode){
	    glad_debug_glColorMaterial(face, mode);
	}
#endif // glColorMaterial

#ifdef glColorP3ui
#undef glColorP3ui
	static inline void glColorP3ui(GLenum type, GLuint color){
	    glad_debug_glColorP3ui(type, color);
	}
#endif // glColorP3ui

#ifdef glColorP3uiv
#undef glColorP3uiv
	static inline void glColorP3uiv(GLenum type, const GLuint *color){
	    glad_debug_glColorP3uiv(type, color);
	}
#endif // glColorP3uiv

#ifdef glColorP4ui
#undef glColorP4ui
	static inline void glColorP4ui(GLenum type, GLuint color){
	    glad_debug_glColorP4ui(type, color);
	}
#endif // glColorP4ui

#ifdef glColorP4uiv
#undef glColorP4uiv
	static inline void glColorP4uiv(GLenum type, const GLuint *color){
	    glad_debug_glColorP4uiv(type, color);
	}
#endif // glColorP4uiv

#ifdef glColorTableParameterfv
#undef glColorTableParameterfv
	static inline void glColorTableParameterfv(GLenum target, GLenum pname, const GLfloat *params){
	    glad_debug_glColorTableParameterfv(target, pname, params);
	}
#endif // glColorTableParameterfv

#ifdef glColorTableParameterfvSGI
#undef glColorTableParameterfvSGI
	static inline void glColorTableParameterfvSGI(GLenum target, GLenum pname, const GLfloat *params){
	    glad_debug_glColorTableParameterfvSGI(target, pname, params);
	}
#endif // glColorTableParameterfvSGI

#ifdef glColorTableParameteriv
#undef glColorTableParameteriv
	static inline void glColorTableParameteriv(GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glColorTableParameteriv(target, pname, params);
	}
#endif // glColorTableParameteriv

#ifdef glColorTableParameterivSGI
#undef glColorTableParameterivSGI
	static inline void glColorTableParameterivSGI(GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glColorTableParameterivSGI(target, pname, params);
	}
#endif // glColorTableParameterivSGI

#ifdef glCombinerInputNV
#undef glCombinerInputNV
	static inline void glCombinerInputNV(GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage){
	    glad_debug_glCombinerInputNV(stage, portion, variable, input, mapping, componentUsage);
	}
#endif // glCombinerInputNV

#ifdef glCombinerOutputNV
#undef glCombinerOutputNV
	static inline void glCombinerOutputNV(GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum){
	    glad_debug_glCombinerOutputNV(stage, portion, abOutput, cdOutput, sumOutput, scale, bias, abDotProduct, cdDotProduct, muxSum);
	}
#endif // glCombinerOutputNV

#ifdef glCombinerParameterfNV
#undef glCombinerParameterfNV
	static inline void glCombinerParameterfNV(GLenum pname, GLfloat param){
	    glad_debug_glCombinerParameterfNV(pname, param);
	}
#endif // glCombinerParameterfNV

#ifdef glCombinerParameterfvNV
#undef glCombinerParameterfvNV
	static inline void glCombinerParameterfvNV(GLenum pname, const GLfloat *params){
	    glad_debug_glCombinerParameterfvNV(pname, params);
	}
#endif // glCombinerParameterfvNV

#ifdef glCombinerParameteriNV
#undef glCombinerParameteriNV
	static inline void glCombinerParameteriNV(GLenum pname, GLint param){
	    glad_debug_glCombinerParameteriNV(pname, param);
	}
#endif // glCombinerParameteriNV

#ifdef glCombinerParameterivNV
#undef glCombinerParameterivNV
	static inline void glCombinerParameterivNV(GLenum pname, const GLint *params){
	    glad_debug_glCombinerParameterivNV(pname, params);
	}
#endif // glCombinerParameterivNV

#ifdef glCombinerStageParameterfvNV
#undef glCombinerStageParameterfvNV
	static inline void glCombinerStageParameterfvNV(GLenum stage, GLenum pname, const GLfloat *params){
	    glad_debug_glCombinerStageParameterfvNV(stage, pname, params);
	}
#endif // glCombinerStageParameterfvNV

#ifdef glCommandListSegmentsNV
#undef glCommandListSegmentsNV
	static inline void glCommandListSegmentsNV(GLuint list, GLuint segments){
	    glad_debug_glCommandListSegmentsNV(list, segments);
	}
#endif // glCommandListSegmentsNV

#ifdef glCompileCommandListNV
#undef glCompileCommandListNV
	static inline void glCompileCommandListNV(GLuint list){
	    glad_debug_glCompileCommandListNV(list);
	}
#endif // glCompileCommandListNV

#ifdef glCompileShader
#undef glCompileShader
	static inline void glCompileShader(GLuint shader){
	    glad_debug_glCompileShader(shader);
	}
#endif // glCompileShader

#ifdef glCompileShaderARB
#undef glCompileShaderARB
	static inline void glCompileShaderARB(GLhandleARB shaderObj){
	    glad_debug_glCompileShaderARB(shaderObj);
	}
#endif // glCompileShaderARB

#ifdef glCompileShaderIncludeARB
#undef glCompileShaderIncludeARB
	static inline void glCompileShaderIncludeARB(GLuint shader, GLsizei count, const GLchar *const*path, const GLint *length){
	    glad_debug_glCompileShaderIncludeARB(shader, count, path, length);
	}
#endif // glCompileShaderIncludeARB

#ifdef glConservativeRasterParameterfNV
#undef glConservativeRasterParameterfNV
	static inline void glConservativeRasterParameterfNV(GLenum pname, GLfloat value){
	    glad_debug_glConservativeRasterParameterfNV(pname, value);
	}
#endif // glConservativeRasterParameterfNV

#ifdef glConservativeRasterParameteriNV
#undef glConservativeRasterParameteriNV
	static inline void glConservativeRasterParameteriNV(GLenum pname, GLint param){
	    glad_debug_glConservativeRasterParameteriNV(pname, param);
	}
#endif // glConservativeRasterParameteriNV

#ifdef glConvolutionParameterf
#undef glConvolutionParameterf
	static inline void glConvolutionParameterf(GLenum target, GLenum pname, GLfloat params){
	    glad_debug_glConvolutionParameterf(target, pname, params);
	}
#endif // glConvolutionParameterf

#ifdef glConvolutionParameterfEXT
#undef glConvolutionParameterfEXT
	static inline void glConvolutionParameterfEXT(GLenum target, GLenum pname, GLfloat params){
	    glad_debug_glConvolutionParameterfEXT(target, pname, params);
	}
#endif // glConvolutionParameterfEXT

#ifdef glConvolutionParameterfv
#undef glConvolutionParameterfv
	static inline void glConvolutionParameterfv(GLenum target, GLenum pname, const GLfloat *params){
	    glad_debug_glConvolutionParameterfv(target, pname, params);
	}
#endif // glConvolutionParameterfv

#ifdef glConvolutionParameterfvEXT
#undef glConvolutionParameterfvEXT
	static inline void glConvolutionParameterfvEXT(GLenum target, GLenum pname, const GLfloat *params){
	    glad_debug_glConvolutionParameterfvEXT(target, pname, params);
	}
#endif // glConvolutionParameterfvEXT

#ifdef glConvolutionParameteri
#undef glConvolutionParameteri
	static inline void glConvolutionParameteri(GLenum target, GLenum pname, GLint params){
	    glad_debug_glConvolutionParameteri(target, pname, params);
	}
#endif // glConvolutionParameteri

#ifdef glConvolutionParameteriEXT
#undef glConvolutionParameteriEXT
	static inline void glConvolutionParameteriEXT(GLenum target, GLenum pname, GLint params){
	    glad_debug_glConvolutionParameteriEXT(target, pname, params);
	}
#endif // glConvolutionParameteriEXT

#ifdef glConvolutionParameteriv
#undef glConvolutionParameteriv
	static inline void glConvolutionParameteriv(GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glConvolutionParameteriv(target, pname, params);
	}
#endif // glConvolutionParameteriv

#ifdef glConvolutionParameterivEXT
#undef glConvolutionParameterivEXT
	static inline void glConvolutionParameterivEXT(GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glConvolutionParameterivEXT(target, pname, params);
	}
#endif // glConvolutionParameterivEXT

#ifdef glConvolutionParameterxOES
#undef glConvolutionParameterxOES
	static inline void glConvolutionParameterxOES(GLenum target, GLenum pname, GLfixed param){
	    glad_debug_glConvolutionParameterxOES(target, pname, param);
	}
#endif // glConvolutionParameterxOES

#ifdef glConvolutionParameterxvOES
#undef glConvolutionParameterxvOES
	static inline void glConvolutionParameterxvOES(GLenum target, GLenum pname, const GLfixed *params){
	    glad_debug_glConvolutionParameterxvOES(target, pname, params);
	}
#endif // glConvolutionParameterxvOES

#ifdef glCopyBufferSubData
#undef glCopyBufferSubData
	static inline void glCopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size){
	    glad_debug_glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
	}
#endif // glCopyBufferSubData

#ifdef glCopyBufferSubDataNV
#undef glCopyBufferSubDataNV
	static inline void glCopyBufferSubDataNV(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size){
	    glad_debug_glCopyBufferSubDataNV(readTarget, writeTarget, readOffset, writeOffset, size);
	}
#endif // glCopyBufferSubDataNV

#ifdef glCopyColorSubTable
#undef glCopyColorSubTable
	static inline void glCopyColorSubTable(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width){
	    glad_debug_glCopyColorSubTable(target, start, x, y, width);
	}
#endif // glCopyColorSubTable

#ifdef glCopyColorSubTableEXT
#undef glCopyColorSubTableEXT
	static inline void glCopyColorSubTableEXT(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width){
	    glad_debug_glCopyColorSubTableEXT(target, start, x, y, width);
	}
#endif // glCopyColorSubTableEXT

#ifdef glCopyColorTable
#undef glCopyColorTable
	static inline void glCopyColorTable(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width){
	    glad_debug_glCopyColorTable(target, internalformat, x, y, width);
	}
#endif // glCopyColorTable

#ifdef glCopyColorTableSGI
#undef glCopyColorTableSGI
	static inline void glCopyColorTableSGI(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width){
	    glad_debug_glCopyColorTableSGI(target, internalformat, x, y, width);
	}
#endif // glCopyColorTableSGI

#ifdef glCopyConvolutionFilter1D
#undef glCopyConvolutionFilter1D
	static inline void glCopyConvolutionFilter1D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width){
	    glad_debug_glCopyConvolutionFilter1D(target, internalformat, x, y, width);
	}
#endif // glCopyConvolutionFilter1D

#ifdef glCopyConvolutionFilter1DEXT
#undef glCopyConvolutionFilter1DEXT
	static inline void glCopyConvolutionFilter1DEXT(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width){
	    glad_debug_glCopyConvolutionFilter1DEXT(target, internalformat, x, y, width);
	}
#endif // glCopyConvolutionFilter1DEXT

#ifdef glCopyConvolutionFilter2D
#undef glCopyConvolutionFilter2D
	static inline void glCopyConvolutionFilter2D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glCopyConvolutionFilter2D(target, internalformat, x, y, width, height);
	}
#endif // glCopyConvolutionFilter2D

#ifdef glCopyConvolutionFilter2DEXT
#undef glCopyConvolutionFilter2DEXT
	static inline void glCopyConvolutionFilter2DEXT(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glCopyConvolutionFilter2DEXT(target, internalformat, x, y, width, height);
	}
#endif // glCopyConvolutionFilter2DEXT

#ifdef glCopyImageSubData
#undef glCopyImageSubData
	static inline void glCopyImageSubData(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth){
	    glad_debug_glCopyImageSubData(srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);
	}
#endif // glCopyImageSubData

#ifdef glCopyImageSubDataEXT
#undef glCopyImageSubDataEXT
	static inline void glCopyImageSubDataEXT(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth){
	    glad_debug_glCopyImageSubDataEXT(srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);
	}
#endif // glCopyImageSubDataEXT

#ifdef glCopyImageSubDataNV
#undef glCopyImageSubDataNV
	static inline void glCopyImageSubDataNV(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth){
	    glad_debug_glCopyImageSubDataNV(srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);
	}
#endif // glCopyImageSubDataNV

#ifdef glCopyImageSubDataOES
#undef glCopyImageSubDataOES
	static inline void glCopyImageSubDataOES(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth){
	    glad_debug_glCopyImageSubDataOES(srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);
	}
#endif // glCopyImageSubDataOES

#ifdef glCopyMultiTexImage1DEXT
#undef glCopyMultiTexImage1DEXT
	static inline void glCopyMultiTexImage1DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border){
	    glad_debug_glCopyMultiTexImage1DEXT(texunit, target, level, internalformat, x, y, width, border);
	}
#endif // glCopyMultiTexImage1DEXT

#ifdef glCopyMultiTexImage2DEXT
#undef glCopyMultiTexImage2DEXT
	static inline void glCopyMultiTexImage2DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border){
	    glad_debug_glCopyMultiTexImage2DEXT(texunit, target, level, internalformat, x, y, width, height, border);
	}
#endif // glCopyMultiTexImage2DEXT

#ifdef glCopyMultiTexSubImage1DEXT
#undef glCopyMultiTexSubImage1DEXT
	static inline void glCopyMultiTexSubImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width){
	    glad_debug_glCopyMultiTexSubImage1DEXT(texunit, target, level, xoffset, x, y, width);
	}
#endif // glCopyMultiTexSubImage1DEXT

#ifdef glCopyMultiTexSubImage2DEXT
#undef glCopyMultiTexSubImage2DEXT
	static inline void glCopyMultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glCopyMultiTexSubImage2DEXT(texunit, target, level, xoffset, yoffset, x, y, width, height);
	}
#endif // glCopyMultiTexSubImage2DEXT

#ifdef glCopyMultiTexSubImage3DEXT
#undef glCopyMultiTexSubImage3DEXT
	static inline void glCopyMultiTexSubImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glCopyMultiTexSubImage3DEXT(texunit, target, level, xoffset, yoffset, zoffset, x, y, width, height);
	}
#endif // glCopyMultiTexSubImage3DEXT

#ifdef glCopyNamedBufferSubData
#undef glCopyNamedBufferSubData
	static inline void glCopyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size){
	    glad_debug_glCopyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset, size);
	}
#endif // glCopyNamedBufferSubData

#ifdef glCopyPathNV
#undef glCopyPathNV
	static inline void glCopyPathNV(GLuint resultPath, GLuint srcPath){
	    glad_debug_glCopyPathNV(resultPath, srcPath);
	}
#endif // glCopyPathNV

#ifdef glCopyPixels
#undef glCopyPixels
	static inline void glCopyPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type){
	    glad_debug_glCopyPixels(x, y, width, height, type);
	}
#endif // glCopyPixels

#ifdef glCopyTexImage1D
#undef glCopyTexImage1D
	static inline void glCopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border){
	    glad_debug_glCopyTexImage1D(target, level, internalformat, x, y, width, border);
	}
#endif // glCopyTexImage1D

#ifdef glCopyTexImage1DEXT
#undef glCopyTexImage1DEXT
	static inline void glCopyTexImage1DEXT(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border){
	    glad_debug_glCopyTexImage1DEXT(target, level, internalformat, x, y, width, border);
	}
#endif // glCopyTexImage1DEXT

#ifdef glCopyTexImage2D
#undef glCopyTexImage2D
	static inline void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border){
	    glad_debug_glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
	}
#endif // glCopyTexImage2D

#ifdef glCopyTexImage2DEXT
#undef glCopyTexImage2DEXT
	static inline void glCopyTexImage2DEXT(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border){
	    glad_debug_glCopyTexImage2DEXT(target, level, internalformat, x, y, width, height, border);
	}
#endif // glCopyTexImage2DEXT

#ifdef glCopyTexSubImage1D
#undef glCopyTexSubImage1D
	static inline void glCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width){
	    glad_debug_glCopyTexSubImage1D(target, level, xoffset, x, y, width);
	}
#endif // glCopyTexSubImage1D

#ifdef glCopyTexSubImage1DEXT
#undef glCopyTexSubImage1DEXT
	static inline void glCopyTexSubImage1DEXT(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width){
	    glad_debug_glCopyTexSubImage1DEXT(target, level, xoffset, x, y, width);
	}
#endif // glCopyTexSubImage1DEXT

#ifdef glCopyTexSubImage2D
#undef glCopyTexSubImage2D
	static inline void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
	}
#endif // glCopyTexSubImage2D

#ifdef glCopyTexSubImage2DEXT
#undef glCopyTexSubImage2DEXT
	static inline void glCopyTexSubImage2DEXT(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glCopyTexSubImage2DEXT(target, level, xoffset, yoffset, x, y, width, height);
	}
#endif // glCopyTexSubImage2DEXT

#ifdef glCopyTexSubImage3D
#undef glCopyTexSubImage3D
	static inline void glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
	}
#endif // glCopyTexSubImage3D

#ifdef glCopyTexSubImage3DEXT
#undef glCopyTexSubImage3DEXT
	static inline void glCopyTexSubImage3DEXT(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glCopyTexSubImage3DEXT(target, level, xoffset, yoffset, zoffset, x, y, width, height);
	}
#endif // glCopyTexSubImage3DEXT

#ifdef glCopyTexSubImage3DOES
#undef glCopyTexSubImage3DOES
	static inline void glCopyTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glCopyTexSubImage3DOES(target, level, xoffset, yoffset, zoffset, x, y, width, height);
	}
#endif // glCopyTexSubImage3DOES

#ifdef glCopyTextureImage1DEXT
#undef glCopyTextureImage1DEXT
	static inline void glCopyTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border){
	    glad_debug_glCopyTextureImage1DEXT(texture, target, level, internalformat, x, y, width, border);
	}
#endif // glCopyTextureImage1DEXT

#ifdef glCopyTextureImage2DEXT
#undef glCopyTextureImage2DEXT
	static inline void glCopyTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border){
	    glad_debug_glCopyTextureImage2DEXT(texture, target, level, internalformat, x, y, width, height, border);
	}
#endif // glCopyTextureImage2DEXT

#ifdef glCopyTextureLevelsAPPLE
#undef glCopyTextureLevelsAPPLE
	static inline void glCopyTextureLevelsAPPLE(GLuint destinationTexture, GLuint sourceTexture, GLint sourceBaseLevel, GLsizei sourceLevelCount){
	    glad_debug_glCopyTextureLevelsAPPLE(destinationTexture, sourceTexture, sourceBaseLevel, sourceLevelCount);
	}
#endif // glCopyTextureLevelsAPPLE

#ifdef glCopyTextureSubImage1D
#undef glCopyTextureSubImage1D
	static inline void glCopyTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width){
	    glad_debug_glCopyTextureSubImage1D(texture, level, xoffset, x, y, width);
	}
#endif // glCopyTextureSubImage1D

#ifdef glCopyTextureSubImage1DEXT
#undef glCopyTextureSubImage1DEXT
	static inline void glCopyTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width){
	    glad_debug_glCopyTextureSubImage1DEXT(texture, target, level, xoffset, x, y, width);
	}
#endif // glCopyTextureSubImage1DEXT

#ifdef glCopyTextureSubImage2D
#undef glCopyTextureSubImage2D
	static inline void glCopyTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glCopyTextureSubImage2D(texture, level, xoffset, yoffset, x, y, width, height);
	}
#endif // glCopyTextureSubImage2D

#ifdef glCopyTextureSubImage2DEXT
#undef glCopyTextureSubImage2DEXT
	static inline void glCopyTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glCopyTextureSubImage2DEXT(texture, target, level, xoffset, yoffset, x, y, width, height);
	}
#endif // glCopyTextureSubImage2DEXT

#ifdef glCopyTextureSubImage3D
#undef glCopyTextureSubImage3D
	static inline void glCopyTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glCopyTextureSubImage3D(texture, level, xoffset, yoffset, zoffset, x, y, width, height);
	}
#endif // glCopyTextureSubImage3D

#ifdef glCopyTextureSubImage3DEXT
#undef glCopyTextureSubImage3DEXT
	static inline void glCopyTextureSubImage3DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glCopyTextureSubImage3DEXT(texture, target, level, xoffset, yoffset, zoffset, x, y, width, height);
	}
#endif // glCopyTextureSubImage3DEXT

#ifdef glCoverFillPathNV
#undef glCoverFillPathNV
	static inline void glCoverFillPathNV(GLuint path, GLenum coverMode){
	    glad_debug_glCoverFillPathNV(path, coverMode);
	}
#endif // glCoverFillPathNV

#ifdef glCoverStrokePathNV
#undef glCoverStrokePathNV
	static inline void glCoverStrokePathNV(GLuint path, GLenum coverMode){
	    glad_debug_glCoverStrokePathNV(path, coverMode);
	}
#endif // glCoverStrokePathNV

#ifdef glCoverageMaskNV
#undef glCoverageMaskNV
	static inline void glCoverageMaskNV(GLboolean mask){
	    glad_debug_glCoverageMaskNV(mask);
	}
#endif // glCoverageMaskNV

#ifdef glCoverageModulationNV
#undef glCoverageModulationNV
	static inline void glCoverageModulationNV(GLenum components){
	    glad_debug_glCoverageModulationNV(components);
	}
#endif // glCoverageModulationNV

#ifdef glCoverageModulationTableNV
#undef glCoverageModulationTableNV
	static inline void glCoverageModulationTableNV(GLsizei n, const GLfloat *v){
	    glad_debug_glCoverageModulationTableNV(n, v);
	}
#endif // glCoverageModulationTableNV

#ifdef glCoverageOperationNV
#undef glCoverageOperationNV
	static inline void glCoverageOperationNV(GLenum operation){
	    glad_debug_glCoverageOperationNV(operation);
	}
#endif // glCoverageOperationNV

#ifdef glCreateBuffers
#undef glCreateBuffers
	static inline void glCreateBuffers(GLsizei n, GLuint *buffers){
	    glad_debug_glCreateBuffers(n, buffers);
	}
#endif // glCreateBuffers

#ifdef glCreateCommandListsNV
#undef glCreateCommandListsNV
	static inline void glCreateCommandListsNV(GLsizei n, GLuint *lists){
	    glad_debug_glCreateCommandListsNV(n, lists);
	}
#endif // glCreateCommandListsNV

#ifdef glCreateFramebuffers
#undef glCreateFramebuffers
	static inline void glCreateFramebuffers(GLsizei n, GLuint *framebuffers){
	    glad_debug_glCreateFramebuffers(n, framebuffers);
	}
#endif // glCreateFramebuffers

#ifdef glCreateMemoryObjectsEXT
#undef glCreateMemoryObjectsEXT
	static inline void glCreateMemoryObjectsEXT(GLsizei n, GLuint *memoryObjects){
	    glad_debug_glCreateMemoryObjectsEXT(n, memoryObjects);
	}
#endif // glCreateMemoryObjectsEXT

#ifdef glCreatePerfQueryINTEL
#undef glCreatePerfQueryINTEL
	static inline void glCreatePerfQueryINTEL(GLuint queryId, GLuint *queryHandle){
	    glad_debug_glCreatePerfQueryINTEL(queryId, queryHandle);
	}
#endif // glCreatePerfQueryINTEL

#ifdef glCreateProgramPipelines
#undef glCreateProgramPipelines
	static inline void glCreateProgramPipelines(GLsizei n, GLuint *pipelines){
	    glad_debug_glCreateProgramPipelines(n, pipelines);
	}
#endif // glCreateProgramPipelines

#ifdef glCreateQueries
#undef glCreateQueries
	static inline void glCreateQueries(GLenum target, GLsizei n, GLuint *ids){
	    glad_debug_glCreateQueries(target, n, ids);
	}
#endif // glCreateQueries

#ifdef glCreateRenderbuffers
#undef glCreateRenderbuffers
	static inline void glCreateRenderbuffers(GLsizei n, GLuint *renderbuffers){
	    glad_debug_glCreateRenderbuffers(n, renderbuffers);
	}
#endif // glCreateRenderbuffers

#ifdef glCreateSamplers
#undef glCreateSamplers
	static inline void glCreateSamplers(GLsizei n, GLuint *samplers){
	    glad_debug_glCreateSamplers(n, samplers);
	}
#endif // glCreateSamplers

#ifdef glCreateSemaphoresNV
#undef glCreateSemaphoresNV
	static inline void glCreateSemaphoresNV(GLsizei n, GLuint *semaphores){
	    glad_debug_glCreateSemaphoresNV(n, semaphores);
	}
#endif // glCreateSemaphoresNV

#ifdef glCreateStatesNV
#undef glCreateStatesNV
	static inline void glCreateStatesNV(GLsizei n, GLuint *states){
	    glad_debug_glCreateStatesNV(n, states);
	}
#endif // glCreateStatesNV

#ifdef glCreateTextures
#undef glCreateTextures
	static inline void glCreateTextures(GLenum target, GLsizei n, GLuint *textures){
	    glad_debug_glCreateTextures(target, n, textures);
	}
#endif // glCreateTextures

#ifdef glCreateTransformFeedbacks
#undef glCreateTransformFeedbacks
	static inline void glCreateTransformFeedbacks(GLsizei n, GLuint *ids){
	    glad_debug_glCreateTransformFeedbacks(n, ids);
	}
#endif // glCreateTransformFeedbacks

#ifdef glCreateVertexArrays
#undef glCreateVertexArrays
	static inline void glCreateVertexArrays(GLsizei n, GLuint *arrays){
	    glad_debug_glCreateVertexArrays(n, arrays);
	}
#endif // glCreateVertexArrays

#ifdef glCullFace
#undef glCullFace
	static inline void glCullFace(GLenum mode){
	    glad_debug_glCullFace(mode);
	}
#endif // glCullFace

#ifdef glCullParameterdvEXT
#undef glCullParameterdvEXT
	static inline void glCullParameterdvEXT(GLenum pname, GLdouble *params){
	    glad_debug_glCullParameterdvEXT(pname, params);
	}
#endif // glCullParameterdvEXT

#ifdef glCullParameterfvEXT
#undef glCullParameterfvEXT
	static inline void glCullParameterfvEXT(GLenum pname, GLfloat *params){
	    glad_debug_glCullParameterfvEXT(pname, params);
	}
#endif // glCullParameterfvEXT

#ifdef glCurrentPaletteMatrixARB
#undef glCurrentPaletteMatrixARB
	static inline void glCurrentPaletteMatrixARB(GLint index){
	    glad_debug_glCurrentPaletteMatrixARB(index);
	}
#endif // glCurrentPaletteMatrixARB

#ifdef glCurrentPaletteMatrixOES
#undef glCurrentPaletteMatrixOES
	static inline void glCurrentPaletteMatrixOES(GLuint matrixpaletteindex){
	    glad_debug_glCurrentPaletteMatrixOES(matrixpaletteindex);
	}
#endif // glCurrentPaletteMatrixOES

#ifdef glDebugMessageControl
#undef glDebugMessageControl
	static inline void glDebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled){
	    glad_debug_glDebugMessageControl(source, type, severity, count, ids, enabled);
	}
#endif // glDebugMessageControl

#ifdef glDebugMessageControlARB
#undef glDebugMessageControlARB
	static inline void glDebugMessageControlARB(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled){
	    glad_debug_glDebugMessageControlARB(source, type, severity, count, ids, enabled);
	}
#endif // glDebugMessageControlARB

#ifdef glDebugMessageControlKHR
#undef glDebugMessageControlKHR
	static inline void glDebugMessageControlKHR(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled){
	    glad_debug_glDebugMessageControlKHR(source, type, severity, count, ids, enabled);
	}
#endif // glDebugMessageControlKHR

#ifdef glDebugMessageEnableAMD
#undef glDebugMessageEnableAMD
	static inline void glDebugMessageEnableAMD(GLenum category, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled){
	    glad_debug_glDebugMessageEnableAMD(category, severity, count, ids, enabled);
	}
#endif // glDebugMessageEnableAMD

#ifdef glDebugMessageInsert
#undef glDebugMessageInsert
	static inline void glDebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf){
	    glad_debug_glDebugMessageInsert(source, type, id, severity, length, buf);
	}
#endif // glDebugMessageInsert

#ifdef glDebugMessageInsertAMD
#undef glDebugMessageInsertAMD
	static inline void glDebugMessageInsertAMD(GLenum category, GLenum severity, GLuint id, GLsizei length, const GLchar *buf){
	    glad_debug_glDebugMessageInsertAMD(category, severity, id, length, buf);
	}
#endif // glDebugMessageInsertAMD

#ifdef glDebugMessageInsertARB
#undef glDebugMessageInsertARB
	static inline void glDebugMessageInsertARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf){
	    glad_debug_glDebugMessageInsertARB(source, type, id, severity, length, buf);
	}
#endif // glDebugMessageInsertARB

#ifdef glDebugMessageInsertKHR
#undef glDebugMessageInsertKHR
	static inline void glDebugMessageInsertKHR(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf){
	    glad_debug_glDebugMessageInsertKHR(source, type, id, severity, length, buf);
	}
#endif // glDebugMessageInsertKHR

#ifdef glDeformSGIX
#undef glDeformSGIX
	static inline void glDeformSGIX(GLbitfield mask){
	    glad_debug_glDeformSGIX(mask);
	}
#endif // glDeformSGIX

#ifdef glDeformationMap3dSGIX
#undef glDeformationMap3dSGIX
	static inline void glDeformationMap3dSGIX(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble w1, GLdouble w2, GLint wstride, GLint worder, const GLdouble *points){
	    glad_debug_glDeformationMap3dSGIX(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, w1, w2, wstride, worder, points);
	}
#endif // glDeformationMap3dSGIX

#ifdef glDeformationMap3fSGIX
#undef glDeformationMap3fSGIX
	static inline void glDeformationMap3fSGIX(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat w1, GLfloat w2, GLint wstride, GLint worder, const GLfloat *points){
	    glad_debug_glDeformationMap3fSGIX(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, w1, w2, wstride, worder, points);
	}
#endif // glDeformationMap3fSGIX

#ifdef glDeleteAsyncMarkersSGIX
#undef glDeleteAsyncMarkersSGIX
	static inline void glDeleteAsyncMarkersSGIX(GLuint marker, GLsizei range){
	    glad_debug_glDeleteAsyncMarkersSGIX(marker, range);
	}
#endif // glDeleteAsyncMarkersSGIX

#ifdef glDeleteBuffers
#undef glDeleteBuffers
	static inline void glDeleteBuffers(GLsizei n, const GLuint *buffers){
	    glad_debug_glDeleteBuffers(n, buffers);
	}
#endif // glDeleteBuffers

#ifdef glDeleteBuffersARB
#undef glDeleteBuffersARB
	static inline void glDeleteBuffersARB(GLsizei n, const GLuint *buffers){
	    glad_debug_glDeleteBuffersARB(n, buffers);
	}
#endif // glDeleteBuffersARB

#ifdef glDeleteCommandListsNV
#undef glDeleteCommandListsNV
	static inline void glDeleteCommandListsNV(GLsizei n, const GLuint *lists){
	    glad_debug_glDeleteCommandListsNV(n, lists);
	}
#endif // glDeleteCommandListsNV

#ifdef glDeleteFencesAPPLE
#undef glDeleteFencesAPPLE
	static inline void glDeleteFencesAPPLE(GLsizei n, const GLuint *fences){
	    glad_debug_glDeleteFencesAPPLE(n, fences);
	}
#endif // glDeleteFencesAPPLE

#ifdef glDeleteFencesNV
#undef glDeleteFencesNV
	static inline void glDeleteFencesNV(GLsizei n, const GLuint *fences){
	    glad_debug_glDeleteFencesNV(n, fences);
	}
#endif // glDeleteFencesNV

#ifdef glDeleteFragmentShaderATI
#undef glDeleteFragmentShaderATI
	static inline void glDeleteFragmentShaderATI(GLuint id){
	    glad_debug_glDeleteFragmentShaderATI(id);
	}
#endif // glDeleteFragmentShaderATI

#ifdef glDeleteFramebuffers
#undef glDeleteFramebuffers
	static inline void glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers){
	    glad_debug_glDeleteFramebuffers(n, framebuffers);
	}
#endif // glDeleteFramebuffers

#ifdef glDeleteFramebuffersEXT
#undef glDeleteFramebuffersEXT
	static inline void glDeleteFramebuffersEXT(GLsizei n, const GLuint *framebuffers){
	    glad_debug_glDeleteFramebuffersEXT(n, framebuffers);
	}
#endif // glDeleteFramebuffersEXT

#ifdef glDeleteFramebuffersOES
#undef glDeleteFramebuffersOES
	static inline void glDeleteFramebuffersOES(GLsizei n, const GLuint *framebuffers){
	    glad_debug_glDeleteFramebuffersOES(n, framebuffers);
	}
#endif // glDeleteFramebuffersOES

#ifdef glDeleteLists
#undef glDeleteLists
	static inline void glDeleteLists(GLuint list, GLsizei range){
	    glad_debug_glDeleteLists(list, range);
	}
#endif // glDeleteLists

#ifdef glDeleteMemoryObjectsEXT
#undef glDeleteMemoryObjectsEXT
	static inline void glDeleteMemoryObjectsEXT(GLsizei n, const GLuint *memoryObjects){
	    glad_debug_glDeleteMemoryObjectsEXT(n, memoryObjects);
	}
#endif // glDeleteMemoryObjectsEXT

#ifdef glDeleteNamedStringARB
#undef glDeleteNamedStringARB
	static inline void glDeleteNamedStringARB(GLint namelen, const GLchar *name){
	    glad_debug_glDeleteNamedStringARB(namelen, name);
	}
#endif // glDeleteNamedStringARB

#ifdef glDeleteNamesAMD
#undef glDeleteNamesAMD
	static inline void glDeleteNamesAMD(GLenum identifier, GLuint num, const GLuint *names){
	    glad_debug_glDeleteNamesAMD(identifier, num, names);
	}
#endif // glDeleteNamesAMD

#ifdef glDeleteObjectARB
#undef glDeleteObjectARB
	static inline void glDeleteObjectARB(GLhandleARB obj){
	    glad_debug_glDeleteObjectARB(obj);
	}
#endif // glDeleteObjectARB

#ifdef glDeleteOcclusionQueriesNV
#undef glDeleteOcclusionQueriesNV
	static inline void glDeleteOcclusionQueriesNV(GLsizei n, const GLuint *ids){
	    glad_debug_glDeleteOcclusionQueriesNV(n, ids);
	}
#endif // glDeleteOcclusionQueriesNV

#ifdef glDeletePathsNV
#undef glDeletePathsNV
	static inline void glDeletePathsNV(GLuint path, GLsizei range){
	    glad_debug_glDeletePathsNV(path, range);
	}
#endif // glDeletePathsNV

#ifdef glDeletePerfMonitorsAMD
#undef glDeletePerfMonitorsAMD
	static inline void glDeletePerfMonitorsAMD(GLsizei n, GLuint *monitors){
	    glad_debug_glDeletePerfMonitorsAMD(n, monitors);
	}
#endif // glDeletePerfMonitorsAMD

#ifdef glDeletePerfQueryINTEL
#undef glDeletePerfQueryINTEL
	static inline void glDeletePerfQueryINTEL(GLuint queryHandle){
	    glad_debug_glDeletePerfQueryINTEL(queryHandle);
	}
#endif // glDeletePerfQueryINTEL

#ifdef glDeleteProgram
#undef glDeleteProgram
	static inline void glDeleteProgram(GLuint program){
	    glad_debug_glDeleteProgram(program);
	}
#endif // glDeleteProgram

#ifdef glDeleteProgramPipelines
#undef glDeleteProgramPipelines
	static inline void glDeleteProgramPipelines(GLsizei n, const GLuint *pipelines){
	    glad_debug_glDeleteProgramPipelines(n, pipelines);
	}
#endif // glDeleteProgramPipelines

#ifdef glDeleteProgramPipelinesEXT
#undef glDeleteProgramPipelinesEXT
	static inline void glDeleteProgramPipelinesEXT(GLsizei n, const GLuint *pipelines){
	    glad_debug_glDeleteProgramPipelinesEXT(n, pipelines);
	}
#endif // glDeleteProgramPipelinesEXT

#ifdef glDeleteProgramsARB
#undef glDeleteProgramsARB
	static inline void glDeleteProgramsARB(GLsizei n, const GLuint *programs){
	    glad_debug_glDeleteProgramsARB(n, programs);
	}
#endif // glDeleteProgramsARB

#ifdef glDeleteProgramsNV
#undef glDeleteProgramsNV
	static inline void glDeleteProgramsNV(GLsizei n, const GLuint *programs){
	    glad_debug_glDeleteProgramsNV(n, programs);
	}
#endif // glDeleteProgramsNV

#ifdef glDeleteQueries
#undef glDeleteQueries
	static inline void glDeleteQueries(GLsizei n, const GLuint *ids){
	    glad_debug_glDeleteQueries(n, ids);
	}
#endif // glDeleteQueries

#ifdef glDeleteQueriesARB
#undef glDeleteQueriesARB
	static inline void glDeleteQueriesARB(GLsizei n, const GLuint *ids){
	    glad_debug_glDeleteQueriesARB(n, ids);
	}
#endif // glDeleteQueriesARB

#ifdef glDeleteQueriesEXT
#undef glDeleteQueriesEXT
	static inline void glDeleteQueriesEXT(GLsizei n, const GLuint *ids){
	    glad_debug_glDeleteQueriesEXT(n, ids);
	}
#endif // glDeleteQueriesEXT

#ifdef glDeleteQueryResourceTagNV
#undef glDeleteQueryResourceTagNV
	static inline void glDeleteQueryResourceTagNV(GLsizei n, const GLint *tagIds){
	    glad_debug_glDeleteQueryResourceTagNV(n, tagIds);
	}
#endif // glDeleteQueryResourceTagNV

#ifdef glDeleteRenderbuffers
#undef glDeleteRenderbuffers
	static inline void glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers){
	    glad_debug_glDeleteRenderbuffers(n, renderbuffers);
	}
#endif // glDeleteRenderbuffers

#ifdef glDeleteRenderbuffersEXT
#undef glDeleteRenderbuffersEXT
	static inline void glDeleteRenderbuffersEXT(GLsizei n, const GLuint *renderbuffers){
	    glad_debug_glDeleteRenderbuffersEXT(n, renderbuffers);
	}
#endif // glDeleteRenderbuffersEXT

#ifdef glDeleteRenderbuffersOES
#undef glDeleteRenderbuffersOES
	static inline void glDeleteRenderbuffersOES(GLsizei n, const GLuint *renderbuffers){
	    glad_debug_glDeleteRenderbuffersOES(n, renderbuffers);
	}
#endif // glDeleteRenderbuffersOES

#ifdef glDeleteSamplers
#undef glDeleteSamplers
	static inline void glDeleteSamplers(GLsizei count, const GLuint *samplers){
	    glad_debug_glDeleteSamplers(count, samplers);
	}
#endif // glDeleteSamplers

#ifdef glDeleteSemaphoresEXT
#undef glDeleteSemaphoresEXT
	static inline void glDeleteSemaphoresEXT(GLsizei n, const GLuint *semaphores){
	    glad_debug_glDeleteSemaphoresEXT(n, semaphores);
	}
#endif // glDeleteSemaphoresEXT

#ifdef glDeleteShader
#undef glDeleteShader
	static inline void glDeleteShader(GLuint shader){
	    glad_debug_glDeleteShader(shader);
	}
#endif // glDeleteShader

#ifdef glDeleteStatesNV
#undef glDeleteStatesNV
	static inline void glDeleteStatesNV(GLsizei n, const GLuint *states){
	    glad_debug_glDeleteStatesNV(n, states);
	}
#endif // glDeleteStatesNV

#ifdef glDeleteSync
#undef glDeleteSync
	static inline void glDeleteSync(GLsync sync){
	    glad_debug_glDeleteSync(sync);
	}
#endif // glDeleteSync

#ifdef glDeleteSyncAPPLE
#undef glDeleteSyncAPPLE
	static inline void glDeleteSyncAPPLE(GLsync sync){
	    glad_debug_glDeleteSyncAPPLE(sync);
	}
#endif // glDeleteSyncAPPLE

#ifdef glDeleteTextures
#undef glDeleteTextures
	static inline void glDeleteTextures(GLsizei n, const GLuint *textures){
	    glad_debug_glDeleteTextures(n, textures);
	}
#endif // glDeleteTextures

#ifdef glDeleteTexturesEXT
#undef glDeleteTexturesEXT
	static inline void glDeleteTexturesEXT(GLsizei n, const GLuint *textures){
	    glad_debug_glDeleteTexturesEXT(n, textures);
	}
#endif // glDeleteTexturesEXT

#ifdef glDeleteTransformFeedbacks
#undef glDeleteTransformFeedbacks
	static inline void glDeleteTransformFeedbacks(GLsizei n, const GLuint *ids){
	    glad_debug_glDeleteTransformFeedbacks(n, ids);
	}
#endif // glDeleteTransformFeedbacks

#ifdef glDeleteTransformFeedbacksNV
#undef glDeleteTransformFeedbacksNV
	static inline void glDeleteTransformFeedbacksNV(GLsizei n, const GLuint *ids){
	    glad_debug_glDeleteTransformFeedbacksNV(n, ids);
	}
#endif // glDeleteTransformFeedbacksNV

#ifdef glDeleteVertexArrays
#undef glDeleteVertexArrays
	static inline void glDeleteVertexArrays(GLsizei n, const GLuint *arrays){
	    glad_debug_glDeleteVertexArrays(n, arrays);
	}
#endif // glDeleteVertexArrays

#ifdef glDeleteVertexArraysAPPLE
#undef glDeleteVertexArraysAPPLE
	static inline void glDeleteVertexArraysAPPLE(GLsizei n, const GLuint *arrays){
	    glad_debug_glDeleteVertexArraysAPPLE(n, arrays);
	}
#endif // glDeleteVertexArraysAPPLE

#ifdef glDeleteVertexArraysOES
#undef glDeleteVertexArraysOES
	static inline void glDeleteVertexArraysOES(GLsizei n, const GLuint *arrays){
	    glad_debug_glDeleteVertexArraysOES(n, arrays);
	}
#endif // glDeleteVertexArraysOES

#ifdef glDeleteVertexShaderEXT
#undef glDeleteVertexShaderEXT
	static inline void glDeleteVertexShaderEXT(GLuint id){
	    glad_debug_glDeleteVertexShaderEXT(id);
	}
#endif // glDeleteVertexShaderEXT

#ifdef glDepthBoundsEXT
#undef glDepthBoundsEXT
	static inline void glDepthBoundsEXT(GLclampd zmin, GLclampd zmax){
	    glad_debug_glDepthBoundsEXT(zmin, zmax);
	}
#endif // glDepthBoundsEXT

#ifdef glDepthBoundsdNV
#undef glDepthBoundsdNV
	static inline void glDepthBoundsdNV(GLdouble zmin, GLdouble zmax){
	    glad_debug_glDepthBoundsdNV(zmin, zmax);
	}
#endif // glDepthBoundsdNV

#ifdef glDepthFunc
#undef glDepthFunc
	static inline void glDepthFunc(GLenum func){
	    glad_debug_glDepthFunc(func);
	}
#endif // glDepthFunc

#ifdef glDepthMask
#undef glDepthMask
	static inline void glDepthMask(GLboolean flag){
	    glad_debug_glDepthMask(flag);
	}
#endif // glDepthMask

#ifdef glDepthRange
#undef glDepthRange
	static inline void glDepthRange(GLdouble n, GLdouble f){
	    glad_debug_glDepthRange(n, f);
	}
#endif // glDepthRange

#ifdef glDepthRangeArraydvNV
#undef glDepthRangeArraydvNV
	static inline void glDepthRangeArraydvNV(GLuint first, GLsizei count, const GLdouble *v){
	    glad_debug_glDepthRangeArraydvNV(first, count, v);
	}
#endif // glDepthRangeArraydvNV

#ifdef glDepthRangeArrayfvNV
#undef glDepthRangeArrayfvNV
	static inline void glDepthRangeArrayfvNV(GLuint first, GLsizei count, const GLfloat *v){
	    glad_debug_glDepthRangeArrayfvNV(first, count, v);
	}
#endif // glDepthRangeArrayfvNV

#ifdef glDepthRangeArrayfvOES
#undef glDepthRangeArrayfvOES
	static inline void glDepthRangeArrayfvOES(GLuint first, GLsizei count, const GLfloat *v){
	    glad_debug_glDepthRangeArrayfvOES(first, count, v);
	}
#endif // glDepthRangeArrayfvOES

#ifdef glDepthRangeArrayv
#undef glDepthRangeArrayv
	static inline void glDepthRangeArrayv(GLuint first, GLsizei count, const GLdouble *v){
	    glad_debug_glDepthRangeArrayv(first, count, v);
	}
#endif // glDepthRangeArrayv

#ifdef glDepthRangeIndexed
#undef glDepthRangeIndexed
	static inline void glDepthRangeIndexed(GLuint index, GLdouble n, GLdouble f){
	    glad_debug_glDepthRangeIndexed(index, n, f);
	}
#endif // glDepthRangeIndexed

#ifdef glDepthRangeIndexeddNV
#undef glDepthRangeIndexeddNV
	static inline void glDepthRangeIndexeddNV(GLuint index, GLdouble n, GLdouble f){
	    glad_debug_glDepthRangeIndexeddNV(index, n, f);
	}
#endif // glDepthRangeIndexeddNV

#ifdef glDepthRangeIndexedfNV
#undef glDepthRangeIndexedfNV
	static inline void glDepthRangeIndexedfNV(GLuint index, GLfloat n, GLfloat f){
	    glad_debug_glDepthRangeIndexedfNV(index, n, f);
	}
#endif // glDepthRangeIndexedfNV

#ifdef glDepthRangeIndexedfOES
#undef glDepthRangeIndexedfOES
	static inline void glDepthRangeIndexedfOES(GLuint index, GLfloat n, GLfloat f){
	    glad_debug_glDepthRangeIndexedfOES(index, n, f);
	}
#endif // glDepthRangeIndexedfOES

#ifdef glDepthRangedNV
#undef glDepthRangedNV
	static inline void glDepthRangedNV(GLdouble zNear, GLdouble zFar){
	    glad_debug_glDepthRangedNV(zNear, zFar);
	}
#endif // glDepthRangedNV

#ifdef glDepthRangef
#undef glDepthRangef
	static inline void glDepthRangef(GLfloat n, GLfloat f){
	    glad_debug_glDepthRangef(n, f);
	}
#endif // glDepthRangef

#ifdef glDepthRangefOES
#undef glDepthRangefOES
	static inline void glDepthRangefOES(GLclampf n, GLclampf f){
	    glad_debug_glDepthRangefOES(n, f);
	}
#endif // glDepthRangefOES

#ifdef glDepthRangex
#undef glDepthRangex
	static inline void glDepthRangex(GLfixed n, GLfixed f){
	    glad_debug_glDepthRangex(n, f);
	}
#endif // glDepthRangex

#ifdef glDepthRangexOES
#undef glDepthRangexOES
	static inline void glDepthRangexOES(GLfixed n, GLfixed f){
	    glad_debug_glDepthRangexOES(n, f);
	}
#endif // glDepthRangexOES

#ifdef glDetachObjectARB
#undef glDetachObjectARB
	static inline void glDetachObjectARB(GLhandleARB containerObj, GLhandleARB attachedObj){
	    glad_debug_glDetachObjectARB(containerObj, attachedObj);
	}
#endif // glDetachObjectARB

#ifdef glDetachShader
#undef glDetachShader
	static inline void glDetachShader(GLuint program, GLuint shader){
	    glad_debug_glDetachShader(program, shader);
	}
#endif // glDetachShader

#ifdef glDetailTexFuncSGIS
#undef glDetailTexFuncSGIS
	static inline void glDetailTexFuncSGIS(GLenum target, GLsizei n, const GLfloat *points){
	    glad_debug_glDetailTexFuncSGIS(target, n, points);
	}
#endif // glDetailTexFuncSGIS

#ifdef glDisable
#undef glDisable
	static inline void glDisable(GLenum cap){
	    glad_debug_glDisable(cap);
	}
#endif // glDisable

#ifdef glDisableClientState
#undef glDisableClientState
	static inline void glDisableClientState(GLenum array){
	    glad_debug_glDisableClientState(array);
	}
#endif // glDisableClientState

#ifdef glDisableClientStateIndexedEXT
#undef glDisableClientStateIndexedEXT
	static inline void glDisableClientStateIndexedEXT(GLenum array, GLuint index){
	    glad_debug_glDisableClientStateIndexedEXT(array, index);
	}
#endif // glDisableClientStateIndexedEXT

#ifdef glDisableClientStateiEXT
#undef glDisableClientStateiEXT
	static inline void glDisableClientStateiEXT(GLenum array, GLuint index){
	    glad_debug_glDisableClientStateiEXT(array, index);
	}
#endif // glDisableClientStateiEXT

#ifdef glDisableDriverControlQCOM
#undef glDisableDriverControlQCOM
	static inline void glDisableDriverControlQCOM(GLuint driverControl){
	    glad_debug_glDisableDriverControlQCOM(driverControl);
	}
#endif // glDisableDriverControlQCOM

#ifdef glDisableIndexedEXT
#undef glDisableIndexedEXT
	static inline void glDisableIndexedEXT(GLenum target, GLuint index){
	    glad_debug_glDisableIndexedEXT(target, index);
	}
#endif // glDisableIndexedEXT

#ifdef glDisableVariantClientStateEXT
#undef glDisableVariantClientStateEXT
	static inline void glDisableVariantClientStateEXT(GLuint id){
	    glad_debug_glDisableVariantClientStateEXT(id);
	}
#endif // glDisableVariantClientStateEXT

#ifdef glDisableVertexArrayAttrib
#undef glDisableVertexArrayAttrib
	static inline void glDisableVertexArrayAttrib(GLuint vaobj, GLuint index){
	    glad_debug_glDisableVertexArrayAttrib(vaobj, index);
	}
#endif // glDisableVertexArrayAttrib

#ifdef glDisableVertexArrayAttribEXT
#undef glDisableVertexArrayAttribEXT
	static inline void glDisableVertexArrayAttribEXT(GLuint vaobj, GLuint index){
	    glad_debug_glDisableVertexArrayAttribEXT(vaobj, index);
	}
#endif // glDisableVertexArrayAttribEXT

#ifdef glDisableVertexArrayEXT
#undef glDisableVertexArrayEXT
	static inline void glDisableVertexArrayEXT(GLuint vaobj, GLenum array){
	    glad_debug_glDisableVertexArrayEXT(vaobj, array);
	}
#endif // glDisableVertexArrayEXT

#ifdef glDisableVertexAttribAPPLE
#undef glDisableVertexAttribAPPLE
	static inline void glDisableVertexAttribAPPLE(GLuint index, GLenum pname){
	    glad_debug_glDisableVertexAttribAPPLE(index, pname);
	}
#endif // glDisableVertexAttribAPPLE

#ifdef glDisableVertexAttribArray
#undef glDisableVertexAttribArray
	static inline void glDisableVertexAttribArray(GLuint index){
	    glad_debug_glDisableVertexAttribArray(index);
	}
#endif // glDisableVertexAttribArray

#ifdef glDisableVertexAttribArrayARB
#undef glDisableVertexAttribArrayARB
	static inline void glDisableVertexAttribArrayARB(GLuint index){
	    glad_debug_glDisableVertexAttribArrayARB(index);
	}
#endif // glDisableVertexAttribArrayARB

#ifdef glDisablei
#undef glDisablei
	static inline void glDisablei(GLenum target, GLuint index){
	    glad_debug_glDisablei(target, index);
	}
#endif // glDisablei

#ifdef glDisableiEXT
#undef glDisableiEXT
	static inline void glDisableiEXT(GLenum target, GLuint index){
	    glad_debug_glDisableiEXT(target, index);
	}
#endif // glDisableiEXT

#ifdef glDisableiNV
#undef glDisableiNV
	static inline void glDisableiNV(GLenum target, GLuint index){
	    glad_debug_glDisableiNV(target, index);
	}
#endif // glDisableiNV

#ifdef glDisableiOES
#undef glDisableiOES
	static inline void glDisableiOES(GLenum target, GLuint index){
	    glad_debug_glDisableiOES(target, index);
	}
#endif // glDisableiOES

#ifdef glDiscardFramebufferEXT
#undef glDiscardFramebufferEXT
	static inline void glDiscardFramebufferEXT(GLenum target, GLsizei numAttachments, const GLenum *attachments){
	    glad_debug_glDiscardFramebufferEXT(target, numAttachments, attachments);
	}
#endif // glDiscardFramebufferEXT

#ifdef glDispatchCompute
#undef glDispatchCompute
	static inline void glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z){
	    glad_debug_glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
	}
#endif // glDispatchCompute

#ifdef glDispatchComputeGroupSizeARB
#undef glDispatchComputeGroupSizeARB
	static inline void glDispatchComputeGroupSizeARB(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLuint group_size_x, GLuint group_size_y, GLuint group_size_z){
	    glad_debug_glDispatchComputeGroupSizeARB(num_groups_x, num_groups_y, num_groups_z, group_size_x, group_size_y, group_size_z);
	}
#endif // glDispatchComputeGroupSizeARB

#ifdef glDispatchComputeIndirect
#undef glDispatchComputeIndirect
	static inline void glDispatchComputeIndirect(GLintptr indirect){
	    glad_debug_glDispatchComputeIndirect(indirect);
	}
#endif // glDispatchComputeIndirect

#ifdef glDrawArrays
#undef glDrawArrays
	static inline void glDrawArrays(GLenum mode, GLint first, GLsizei count){
	    glad_debug_glDrawArrays(mode, first, count);
	}
#endif // glDrawArrays

#ifdef glDrawArraysEXT
#undef glDrawArraysEXT
	static inline void glDrawArraysEXT(GLenum mode, GLint first, GLsizei count){
	    glad_debug_glDrawArraysEXT(mode, first, count);
	}
#endif // glDrawArraysEXT

#ifdef glDrawArraysInstanced
#undef glDrawArraysInstanced
	static inline void glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount){
	    glad_debug_glDrawArraysInstanced(mode, first, count, instancecount);
	}
#endif // glDrawArraysInstanced

#ifdef glDrawArraysInstancedANGLE
#undef glDrawArraysInstancedANGLE
	static inline void glDrawArraysInstancedANGLE(GLenum mode, GLint first, GLsizei count, GLsizei primcount){
	    glad_debug_glDrawArraysInstancedANGLE(mode, first, count, primcount);
	}
#endif // glDrawArraysInstancedANGLE

#ifdef glDrawArraysInstancedARB
#undef glDrawArraysInstancedARB
	static inline void glDrawArraysInstancedARB(GLenum mode, GLint first, GLsizei count, GLsizei primcount){
	    glad_debug_glDrawArraysInstancedARB(mode, first, count, primcount);
	}
#endif // glDrawArraysInstancedARB

#ifdef glDrawArraysInstancedBaseInstance
#undef glDrawArraysInstancedBaseInstance
	static inline void glDrawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance){
	    glad_debug_glDrawArraysInstancedBaseInstance(mode, first, count, instancecount, baseinstance);
	}
#endif // glDrawArraysInstancedBaseInstance

#ifdef glDrawArraysInstancedBaseInstanceEXT
#undef glDrawArraysInstancedBaseInstanceEXT
	static inline void glDrawArraysInstancedBaseInstanceEXT(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance){
	    glad_debug_glDrawArraysInstancedBaseInstanceEXT(mode, first, count, instancecount, baseinstance);
	}
#endif // glDrawArraysInstancedBaseInstanceEXT

#ifdef glDrawArraysInstancedEXT
#undef glDrawArraysInstancedEXT
	static inline void glDrawArraysInstancedEXT(GLenum mode, GLint start, GLsizei count, GLsizei primcount){
	    glad_debug_glDrawArraysInstancedEXT(mode, start, count, primcount);
	}
#endif // glDrawArraysInstancedEXT

#ifdef glDrawArraysInstancedNV
#undef glDrawArraysInstancedNV
	static inline void glDrawArraysInstancedNV(GLenum mode, GLint first, GLsizei count, GLsizei primcount){
	    glad_debug_glDrawArraysInstancedNV(mode, first, count, primcount);
	}
#endif // glDrawArraysInstancedNV

#ifdef glDrawBuffer
#undef glDrawBuffer
	static inline void glDrawBuffer(GLenum buf){
	    glad_debug_glDrawBuffer(buf);
	}
#endif // glDrawBuffer

#ifdef glDrawBuffers
#undef glDrawBuffers
	static inline void glDrawBuffers(GLsizei n, const GLenum *bufs){
	    glad_debug_glDrawBuffers(n, bufs);
	}
#endif // glDrawBuffers

#ifdef glDrawBuffersARB
#undef glDrawBuffersARB
	static inline void glDrawBuffersARB(GLsizei n, const GLenum *bufs){
	    glad_debug_glDrawBuffersARB(n, bufs);
	}
#endif // glDrawBuffersARB

#ifdef glDrawBuffersATI
#undef glDrawBuffersATI
	static inline void glDrawBuffersATI(GLsizei n, const GLenum *bufs){
	    glad_debug_glDrawBuffersATI(n, bufs);
	}
#endif // glDrawBuffersATI

#ifdef glDrawBuffersEXT
#undef glDrawBuffersEXT
	static inline void glDrawBuffersEXT(GLsizei n, const GLenum *bufs){
	    glad_debug_glDrawBuffersEXT(n, bufs);
	}
#endif // glDrawBuffersEXT

#ifdef glDrawBuffersIndexedEXT
#undef glDrawBuffersIndexedEXT
	static inline void glDrawBuffersIndexedEXT(GLint n, const GLenum *location, const GLint *indices){
	    glad_debug_glDrawBuffersIndexedEXT(n, location, indices);
	}
#endif // glDrawBuffersIndexedEXT

#ifdef glDrawBuffersNV
#undef glDrawBuffersNV
	static inline void glDrawBuffersNV(GLsizei n, const GLenum *bufs){
	    glad_debug_glDrawBuffersNV(n, bufs);
	}
#endif // glDrawBuffersNV

#ifdef glDrawCommandsAddressNV
#undef glDrawCommandsAddressNV
	static inline void glDrawCommandsAddressNV(GLenum primitiveMode, const GLuint64 *indirects, const GLsizei *sizes, GLuint count){
	    glad_debug_glDrawCommandsAddressNV(primitiveMode, indirects, sizes, count);
	}
#endif // glDrawCommandsAddressNV

#ifdef glDrawCommandsNV
#undef glDrawCommandsNV
	static inline void glDrawCommandsNV(GLenum primitiveMode, GLuint buffer, const GLintptr *indirects, const GLsizei *sizes, GLuint count){
	    glad_debug_glDrawCommandsNV(primitiveMode, buffer, indirects, sizes, count);
	}
#endif // glDrawCommandsNV

#ifdef glDrawCommandsStatesAddressNV
#undef glDrawCommandsStatesAddressNV
	static inline void glDrawCommandsStatesAddressNV(const GLuint64 *indirects, const GLsizei *sizes, const GLuint *states, const GLuint *fbos, GLuint count){
	    glad_debug_glDrawCommandsStatesAddressNV(indirects, sizes, states, fbos, count);
	}
#endif // glDrawCommandsStatesAddressNV

#ifdef glDrawCommandsStatesNV
#undef glDrawCommandsStatesNV
	static inline void glDrawCommandsStatesNV(GLuint buffer, const GLintptr *indirects, const GLsizei *sizes, const GLuint *states, const GLuint *fbos, GLuint count){
	    glad_debug_glDrawCommandsStatesNV(buffer, indirects, sizes, states, fbos, count);
	}
#endif // glDrawCommandsStatesNV

#ifdef glDrawElementArrayAPPLE
#undef glDrawElementArrayAPPLE
	static inline void glDrawElementArrayAPPLE(GLenum mode, GLint first, GLsizei count){
	    glad_debug_glDrawElementArrayAPPLE(mode, first, count);
	}
#endif // glDrawElementArrayAPPLE

#ifdef glDrawElementArrayATI
#undef glDrawElementArrayATI
	static inline void glDrawElementArrayATI(GLenum mode, GLsizei count){
	    glad_debug_glDrawElementArrayATI(mode, count);
	}
#endif // glDrawElementArrayATI

#ifdef glDrawMeshArraysSUN
#undef glDrawMeshArraysSUN
	static inline void glDrawMeshArraysSUN(GLenum mode, GLint first, GLsizei count, GLsizei width){
	    glad_debug_glDrawMeshArraysSUN(mode, first, count, width);
	}
#endif // glDrawMeshArraysSUN

#ifdef glDrawMeshTasksEXT
#undef glDrawMeshTasksEXT
	static inline void glDrawMeshTasksEXT(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z){
	    glad_debug_glDrawMeshTasksEXT(num_groups_x, num_groups_y, num_groups_z);
	}
#endif // glDrawMeshTasksEXT

#ifdef glDrawMeshTasksNV
#undef glDrawMeshTasksNV
	static inline void glDrawMeshTasksNV(GLuint first, GLuint count){
	    glad_debug_glDrawMeshTasksNV(first, count);
	}
#endif // glDrawMeshTasksNV

#ifdef glDrawMeshTasksIndirectEXT
#undef glDrawMeshTasksIndirectEXT
	static inline void glDrawMeshTasksIndirectEXT(GLintptr indirect){
	    glad_debug_glDrawMeshTasksIndirectEXT(indirect);
	}
#endif // glDrawMeshTasksIndirectEXT

#ifdef glDrawMeshTasksIndirectNV
#undef glDrawMeshTasksIndirectNV
	static inline void glDrawMeshTasksIndirectNV(GLintptr indirect){
	    glad_debug_glDrawMeshTasksIndirectNV(indirect);
	}
#endif // glDrawMeshTasksIndirectNV

#ifdef glDrawRangeElementArrayAPPLE
#undef glDrawRangeElementArrayAPPLE
	static inline void glDrawRangeElementArrayAPPLE(GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count){
	    glad_debug_glDrawRangeElementArrayAPPLE(mode, start, end, first, count);
	}
#endif // glDrawRangeElementArrayAPPLE

#ifdef glDrawRangeElementArrayATI
#undef glDrawRangeElementArrayATI
	static inline void glDrawRangeElementArrayATI(GLenum mode, GLuint start, GLuint end, GLsizei count){
	    glad_debug_glDrawRangeElementArrayATI(mode, start, end, count);
	}
#endif // glDrawRangeElementArrayATI

#ifdef glDrawTexfOES
#undef glDrawTexfOES
	static inline void glDrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height){
	    glad_debug_glDrawTexfOES(x, y, z, width, height);
	}
#endif // glDrawTexfOES

#ifdef glDrawTexfvOES
#undef glDrawTexfvOES
	static inline void glDrawTexfvOES(const GLfloat *coords){
	    glad_debug_glDrawTexfvOES(coords);
	}
#endif // glDrawTexfvOES

#ifdef glDrawTexiOES
#undef glDrawTexiOES
	static inline void glDrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height){
	    glad_debug_glDrawTexiOES(x, y, z, width, height);
	}
#endif // glDrawTexiOES

#ifdef glDrawTexivOES
#undef glDrawTexivOES
	static inline void glDrawTexivOES(const GLint *coords){
	    glad_debug_glDrawTexivOES(coords);
	}
#endif // glDrawTexivOES

#ifdef glDrawTexsOES
#undef glDrawTexsOES
	static inline void glDrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height){
	    glad_debug_glDrawTexsOES(x, y, z, width, height);
	}
#endif // glDrawTexsOES

#ifdef glDrawTexsvOES
#undef glDrawTexsvOES
	static inline void glDrawTexsvOES(const GLshort *coords){
	    glad_debug_glDrawTexsvOES(coords);
	}
#endif // glDrawTexsvOES

#ifdef glDrawTextureNV
#undef glDrawTextureNV
	static inline void glDrawTextureNV(GLuint texture, GLuint sampler, GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1, GLfloat z, GLfloat s0, GLfloat t0, GLfloat s1, GLfloat t1){
	    glad_debug_glDrawTextureNV(texture, sampler, x0, y0, x1, y1, z, s0, t0, s1, t1);
	}
#endif // glDrawTextureNV

#ifdef glDrawTexxOES
#undef glDrawTexxOES
	static inline void glDrawTexxOES(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height){
	    glad_debug_glDrawTexxOES(x, y, z, width, height);
	}
#endif // glDrawTexxOES

#ifdef glDrawTexxvOES
#undef glDrawTexxvOES
	static inline void glDrawTexxvOES(const GLfixed *coords){
	    glad_debug_glDrawTexxvOES(coords);
	}
#endif // glDrawTexxvOES

#ifdef glDrawTransformFeedback
#undef glDrawTransformFeedback
	static inline void glDrawTransformFeedback(GLenum mode, GLuint id){
	    glad_debug_glDrawTransformFeedback(mode, id);
	}
#endif // glDrawTransformFeedback

#ifdef glDrawTransformFeedbackEXT
#undef glDrawTransformFeedbackEXT
	static inline void glDrawTransformFeedbackEXT(GLenum mode, GLuint id){
	    glad_debug_glDrawTransformFeedbackEXT(mode, id);
	}
#endif // glDrawTransformFeedbackEXT

#ifdef glDrawTransformFeedbackInstanced
#undef glDrawTransformFeedbackInstanced
	static inline void glDrawTransformFeedbackInstanced(GLenum mode, GLuint id, GLsizei instancecount){
	    glad_debug_glDrawTransformFeedbackInstanced(mode, id, instancecount);
	}
#endif // glDrawTransformFeedbackInstanced

#ifdef glDrawTransformFeedbackInstancedEXT
#undef glDrawTransformFeedbackInstancedEXT
	static inline void glDrawTransformFeedbackInstancedEXT(GLenum mode, GLuint id, GLsizei instancecount){
	    glad_debug_glDrawTransformFeedbackInstancedEXT(mode, id, instancecount);
	}
#endif // glDrawTransformFeedbackInstancedEXT

#ifdef glDrawTransformFeedbackNV
#undef glDrawTransformFeedbackNV
	static inline void glDrawTransformFeedbackNV(GLenum mode, GLuint id){
	    glad_debug_glDrawTransformFeedbackNV(mode, id);
	}
#endif // glDrawTransformFeedbackNV

#ifdef glDrawTransformFeedbackStream
#undef glDrawTransformFeedbackStream
	static inline void glDrawTransformFeedbackStream(GLenum mode, GLuint id, GLuint stream){
	    glad_debug_glDrawTransformFeedbackStream(mode, id, stream);
	}
#endif // glDrawTransformFeedbackStream

#ifdef glDrawTransformFeedbackStreamInstanced
#undef glDrawTransformFeedbackStreamInstanced
	static inline void glDrawTransformFeedbackStreamInstanced(GLenum mode, GLuint id, GLuint stream, GLsizei instancecount){
	    glad_debug_glDrawTransformFeedbackStreamInstanced(mode, id, stream, instancecount);
	}
#endif // glDrawTransformFeedbackStreamInstanced

#ifdef glEGLImageTargetRenderbufferStorageOES
#undef glEGLImageTargetRenderbufferStorageOES
	static inline void glEGLImageTargetRenderbufferStorageOES(GLenum target, GLeglImageOES image){
	    glad_debug_glEGLImageTargetRenderbufferStorageOES(target, image);
	}
#endif // glEGLImageTargetRenderbufferStorageOES

#ifdef glEGLImageTargetTexStorageEXT
#undef glEGLImageTargetTexStorageEXT
	static inline void glEGLImageTargetTexStorageEXT(GLenum target, GLeglImageOES image, const GLint* attrib_list){
	    glad_debug_glEGLImageTargetTexStorageEXT(target, image, attrib_list);
	}
#endif // glEGLImageTargetTexStorageEXT

#ifdef glEGLImageTargetTexture2DOES
#undef glEGLImageTargetTexture2DOES
	static inline void glEGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image){
	    glad_debug_glEGLImageTargetTexture2DOES(target, image);
	}
#endif // glEGLImageTargetTexture2DOES

#ifdef glEGLImageTargetTextureStorageEXT
#undef glEGLImageTargetTextureStorageEXT
	static inline void glEGLImageTargetTextureStorageEXT(GLuint texture, GLeglImageOES image, const GLint* attrib_list){
	    glad_debug_glEGLImageTargetTextureStorageEXT(texture, image, attrib_list);
	}
#endif // glEGLImageTargetTextureStorageEXT

#ifdef glEdgeFlag
#undef glEdgeFlag
	static inline void glEdgeFlag(GLboolean flag){
	    glad_debug_glEdgeFlag(flag);
	}
#endif // glEdgeFlag

#ifdef glEdgeFlagFormatNV
#undef glEdgeFlagFormatNV
	static inline void glEdgeFlagFormatNV(GLsizei stride){
	    glad_debug_glEdgeFlagFormatNV(stride);
	}
#endif // glEdgeFlagFormatNV

#ifdef glEdgeFlagPointerEXT
#undef glEdgeFlagPointerEXT
	static inline void glEdgeFlagPointerEXT(GLsizei stride, GLsizei count, const GLboolean *pointer){
	    glad_debug_glEdgeFlagPointerEXT(stride, count, pointer);
	}
#endif // glEdgeFlagPointerEXT

#ifdef glEdgeFlagPointerListIBM
#undef glEdgeFlagPointerListIBM
	static inline void glEdgeFlagPointerListIBM(GLint stride, const GLboolean **pointer, GLint ptrstride){
	    glad_debug_glEdgeFlagPointerListIBM(stride, pointer, ptrstride);
	}
#endif // glEdgeFlagPointerListIBM

#ifdef glEdgeFlagv
#undef glEdgeFlagv
	static inline void glEdgeFlagv(const GLboolean *flag){
	    glad_debug_glEdgeFlagv(flag);
	}
#endif // glEdgeFlagv

#ifdef glEnable
#undef glEnable
	static inline void glEnable(GLenum cap){
	    glad_debug_glEnable(cap);
	}
#endif // glEnable

#ifdef glEnableClientState
#undef glEnableClientState
	static inline void glEnableClientState(GLenum array){
	    glad_debug_glEnableClientState(array);
	}
#endif // glEnableClientState

#ifdef glEnableClientStateIndexedEXT
#undef glEnableClientStateIndexedEXT
	static inline void glEnableClientStateIndexedEXT(GLenum array, GLuint index){
	    glad_debug_glEnableClientStateIndexedEXT(array, index);
	}
#endif // glEnableClientStateIndexedEXT

#ifdef glEnableClientStateiEXT
#undef glEnableClientStateiEXT
	static inline void glEnableClientStateiEXT(GLenum array, GLuint index){
	    glad_debug_glEnableClientStateiEXT(array, index);
	}
#endif // glEnableClientStateiEXT

#ifdef glEnableDriverControlQCOM
#undef glEnableDriverControlQCOM
	static inline void glEnableDriverControlQCOM(GLuint driverControl){
	    glad_debug_glEnableDriverControlQCOM(driverControl);
	}
#endif // glEnableDriverControlQCOM

#ifdef glEnableIndexedEXT
#undef glEnableIndexedEXT
	static inline void glEnableIndexedEXT(GLenum target, GLuint index){
	    glad_debug_glEnableIndexedEXT(target, index);
	}
#endif // glEnableIndexedEXT

#ifdef glEnableVariantClientStateEXT
#undef glEnableVariantClientStateEXT
	static inline void glEnableVariantClientStateEXT(GLuint id){
	    glad_debug_glEnableVariantClientStateEXT(id);
	}
#endif // glEnableVariantClientStateEXT

#ifdef glEnableVertexArrayAttrib
#undef glEnableVertexArrayAttrib
	static inline void glEnableVertexArrayAttrib(GLuint vaobj, GLuint index){
	    glad_debug_glEnableVertexArrayAttrib(vaobj, index);
	}
#endif // glEnableVertexArrayAttrib

#ifdef glEnableVertexArrayAttribEXT
#undef glEnableVertexArrayAttribEXT
	static inline void glEnableVertexArrayAttribEXT(GLuint vaobj, GLuint index){
	    glad_debug_glEnableVertexArrayAttribEXT(vaobj, index);
	}
#endif // glEnableVertexArrayAttribEXT

#ifdef glEnableVertexArrayEXT
#undef glEnableVertexArrayEXT
	static inline void glEnableVertexArrayEXT(GLuint vaobj, GLenum array){
	    glad_debug_glEnableVertexArrayEXT(vaobj, array);
	}
#endif // glEnableVertexArrayEXT

#ifdef glEnableVertexAttribAPPLE
#undef glEnableVertexAttribAPPLE
	static inline void glEnableVertexAttribAPPLE(GLuint index, GLenum pname){
	    glad_debug_glEnableVertexAttribAPPLE(index, pname);
	}
#endif // glEnableVertexAttribAPPLE

#ifdef glEnableVertexAttribArray
#undef glEnableVertexAttribArray
	static inline void glEnableVertexAttribArray(GLuint index){
	    glad_debug_glEnableVertexAttribArray(index);
	}
#endif // glEnableVertexAttribArray

#ifdef glEnableVertexAttribArrayARB
#undef glEnableVertexAttribArrayARB
	static inline void glEnableVertexAttribArrayARB(GLuint index){
	    glad_debug_glEnableVertexAttribArrayARB(index);
	}
#endif // glEnableVertexAttribArrayARB

#ifdef glEnablei
#undef glEnablei
	static inline void glEnablei(GLenum target, GLuint index){
	    glad_debug_glEnablei(target, index);
	}
#endif // glEnablei

#ifdef glEnableiEXT
#undef glEnableiEXT
	static inline void glEnableiEXT(GLenum target, GLuint index){
	    glad_debug_glEnableiEXT(target, index);
	}
#endif // glEnableiEXT

#ifdef glEnableiNV
#undef glEnableiNV
	static inline void glEnableiNV(GLenum target, GLuint index){
	    glad_debug_glEnableiNV(target, index);
	}
#endif // glEnableiNV

#ifdef glEnableiOES
#undef glEnableiOES
	static inline void glEnableiOES(GLenum target, GLuint index){
	    glad_debug_glEnableiOES(target, index);
	}
#endif // glEnableiOES

#ifdef glEnd
#undef glEnd
	static inline void glEnd(){
	    glad_debug_glEnd();
	}
#endif // glEnd

#ifdef glEndConditionalRender
#undef glEndConditionalRender
	static inline void glEndConditionalRender(){
	    glad_debug_glEndConditionalRender();
	}
#endif // glEndConditionalRender

#ifdef glEndConditionalRenderNV
#undef glEndConditionalRenderNV
	static inline void glEndConditionalRenderNV(){
	    glad_debug_glEndConditionalRenderNV();
	}
#endif // glEndConditionalRenderNV

#ifdef glEndConditionalRenderNVX
#undef glEndConditionalRenderNVX
	static inline void glEndConditionalRenderNVX(){
	    glad_debug_glEndConditionalRenderNVX();
	}
#endif // glEndConditionalRenderNVX

#ifdef glEndFragmentShaderATI
#undef glEndFragmentShaderATI
	static inline void glEndFragmentShaderATI(){
	    glad_debug_glEndFragmentShaderATI();
	}
#endif // glEndFragmentShaderATI

#ifdef glEndList
#undef glEndList
	static inline void glEndList(){
	    glad_debug_glEndList();
	}
#endif // glEndList

#ifdef glEndOcclusionQueryNV
#undef glEndOcclusionQueryNV
	static inline void glEndOcclusionQueryNV(){
	    glad_debug_glEndOcclusionQueryNV();
	}
#endif // glEndOcclusionQueryNV

#ifdef glEndPerfMonitorAMD
#undef glEndPerfMonitorAMD
	static inline void glEndPerfMonitorAMD(GLuint monitor){
	    glad_debug_glEndPerfMonitorAMD(monitor);
	}
#endif // glEndPerfMonitorAMD

#ifdef glEndPerfQueryINTEL
#undef glEndPerfQueryINTEL
	static inline void glEndPerfQueryINTEL(GLuint queryHandle){
	    glad_debug_glEndPerfQueryINTEL(queryHandle);
	}
#endif // glEndPerfQueryINTEL

#ifdef glEndQuery
#undef glEndQuery
	static inline void glEndQuery(GLenum target){
	    glad_debug_glEndQuery(target);
	}
#endif // glEndQuery

#ifdef glEndQueryARB
#undef glEndQueryARB
	static inline void glEndQueryARB(GLenum target){
	    glad_debug_glEndQueryARB(target);
	}
#endif // glEndQueryARB

#ifdef glEndQueryEXT
#undef glEndQueryEXT
	static inline void glEndQueryEXT(GLenum target){
	    glad_debug_glEndQueryEXT(target);
	}
#endif // glEndQueryEXT

#ifdef glEndQueryIndexed
#undef glEndQueryIndexed
	static inline void glEndQueryIndexed(GLenum target, GLuint index){
	    glad_debug_glEndQueryIndexed(target, index);
	}
#endif // glEndQueryIndexed

#ifdef glEndTilingQCOM
#undef glEndTilingQCOM
	static inline void glEndTilingQCOM(GLbitfield preserveMask){
	    glad_debug_glEndTilingQCOM(preserveMask);
	}
#endif // glEndTilingQCOM

#ifdef glEndTransformFeedback
#undef glEndTransformFeedback
	static inline void glEndTransformFeedback(){
	    glad_debug_glEndTransformFeedback();
	}
#endif // glEndTransformFeedback

#ifdef glEndTransformFeedbackEXT
#undef glEndTransformFeedbackEXT
	static inline void glEndTransformFeedbackEXT(){
	    glad_debug_glEndTransformFeedbackEXT();
	}
#endif // glEndTransformFeedbackEXT

#ifdef glEndTransformFeedbackNV
#undef glEndTransformFeedbackNV
	static inline void glEndTransformFeedbackNV(){
	    glad_debug_glEndTransformFeedbackNV();
	}
#endif // glEndTransformFeedbackNV

#ifdef glEndVertexShaderEXT
#undef glEndVertexShaderEXT
	static inline void glEndVertexShaderEXT(){
	    glad_debug_glEndVertexShaderEXT();
	}
#endif // glEndVertexShaderEXT

#ifdef glEndVideoCaptureNV
#undef glEndVideoCaptureNV
	static inline void glEndVideoCaptureNV(GLuint video_capture_slot){
	    glad_debug_glEndVideoCaptureNV(video_capture_slot);
	}
#endif // glEndVideoCaptureNV

#ifdef glEvalCoord1d
#undef glEvalCoord1d
	static inline void glEvalCoord1d(GLdouble u){
	    glad_debug_glEvalCoord1d(u);
	}
#endif // glEvalCoord1d

#ifdef glEvalCoord1dv
#undef glEvalCoord1dv
	static inline void glEvalCoord1dv(const GLdouble *u){
	    glad_debug_glEvalCoord1dv(u);
	}
#endif // glEvalCoord1dv

#ifdef glEvalCoord1f
#undef glEvalCoord1f
	static inline void glEvalCoord1f(GLfloat u){
	    glad_debug_glEvalCoord1f(u);
	}
#endif // glEvalCoord1f

#ifdef glEvalCoord1fv
#undef glEvalCoord1fv
	static inline void glEvalCoord1fv(const GLfloat *u){
	    glad_debug_glEvalCoord1fv(u);
	}
#endif // glEvalCoord1fv

#ifdef glEvalCoord1xOES
#undef glEvalCoord1xOES
	static inline void glEvalCoord1xOES(GLfixed u){
	    glad_debug_glEvalCoord1xOES(u);
	}
#endif // glEvalCoord1xOES

#ifdef glEvalCoord1xvOES
#undef glEvalCoord1xvOES
	static inline void glEvalCoord1xvOES(const GLfixed *coords){
	    glad_debug_glEvalCoord1xvOES(coords);
	}
#endif // glEvalCoord1xvOES

#ifdef glEvalCoord2d
#undef glEvalCoord2d
	static inline void glEvalCoord2d(GLdouble u, GLdouble v){
	    glad_debug_glEvalCoord2d(u, v);
	}
#endif // glEvalCoord2d

#ifdef glEvalCoord2dv
#undef glEvalCoord2dv
	static inline void glEvalCoord2dv(const GLdouble *u){
	    glad_debug_glEvalCoord2dv(u);
	}
#endif // glEvalCoord2dv

#ifdef glEvalCoord2f
#undef glEvalCoord2f
	static inline void glEvalCoord2f(GLfloat u, GLfloat v){
	    glad_debug_glEvalCoord2f(u, v);
	}
#endif // glEvalCoord2f

#ifdef glEvalCoord2fv
#undef glEvalCoord2fv
	static inline void glEvalCoord2fv(const GLfloat *u){
	    glad_debug_glEvalCoord2fv(u);
	}
#endif // glEvalCoord2fv

#ifdef glEvalCoord2xOES
#undef glEvalCoord2xOES
	static inline void glEvalCoord2xOES(GLfixed u, GLfixed v){
	    glad_debug_glEvalCoord2xOES(u, v);
	}
#endif // glEvalCoord2xOES

#ifdef glEvalCoord2xvOES
#undef glEvalCoord2xvOES
	static inline void glEvalCoord2xvOES(const GLfixed *coords){
	    glad_debug_glEvalCoord2xvOES(coords);
	}
#endif // glEvalCoord2xvOES

#ifdef glEvalMapsNV
#undef glEvalMapsNV
	static inline void glEvalMapsNV(GLenum target, GLenum mode){
	    glad_debug_glEvalMapsNV(target, mode);
	}
#endif // glEvalMapsNV

#ifdef glEvalMesh1
#undef glEvalMesh1
	static inline void glEvalMesh1(GLenum mode, GLint i1, GLint i2){
	    glad_debug_glEvalMesh1(mode, i1, i2);
	}
#endif // glEvalMesh1

#ifdef glEvalMesh2
#undef glEvalMesh2
	static inline void glEvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2){
	    glad_debug_glEvalMesh2(mode, i1, i2, j1, j2);
	}
#endif // glEvalMesh2

#ifdef glEvalPoint1
#undef glEvalPoint1
	static inline void glEvalPoint1(GLint i){
	    glad_debug_glEvalPoint1(i);
	}
#endif // glEvalPoint1

#ifdef glEvalPoint2
#undef glEvalPoint2
	static inline void glEvalPoint2(GLint i, GLint j){
	    glad_debug_glEvalPoint2(i, j);
	}
#endif // glEvalPoint2

#ifdef glEvaluateDepthValuesARB
#undef glEvaluateDepthValuesARB
	static inline void glEvaluateDepthValuesARB(){
	    glad_debug_glEvaluateDepthValuesARB();
	}
#endif // glEvaluateDepthValuesARB

#ifdef glExecuteProgramNV
#undef glExecuteProgramNV
	static inline void glExecuteProgramNV(GLenum target, GLuint id, const GLfloat *params){
	    glad_debug_glExecuteProgramNV(target, id, params);
	}
#endif // glExecuteProgramNV

#ifdef glExtGetBuffersQCOM
#undef glExtGetBuffersQCOM
	static inline void glExtGetBuffersQCOM(GLuint *buffers, GLint maxBuffers, GLint *numBuffers){
	    glad_debug_glExtGetBuffersQCOM(buffers, maxBuffers, numBuffers);
	}
#endif // glExtGetBuffersQCOM

#ifdef glExtGetFramebuffersQCOM
#undef glExtGetFramebuffersQCOM
	static inline void glExtGetFramebuffersQCOM(GLuint *framebuffers, GLint maxFramebuffers, GLint *numFramebuffers){
	    glad_debug_glExtGetFramebuffersQCOM(framebuffers, maxFramebuffers, numFramebuffers);
	}
#endif // glExtGetFramebuffersQCOM

#ifdef glExtGetProgramBinarySourceQCOM
#undef glExtGetProgramBinarySourceQCOM
	static inline void glExtGetProgramBinarySourceQCOM(GLuint program, GLenum shadertype, GLchar *source, GLint *length){
	    glad_debug_glExtGetProgramBinarySourceQCOM(program, shadertype, source, length);
	}
#endif // glExtGetProgramBinarySourceQCOM

#ifdef glExtGetProgramsQCOM
#undef glExtGetProgramsQCOM
	static inline void glExtGetProgramsQCOM(GLuint *programs, GLint maxPrograms, GLint *numPrograms){
	    glad_debug_glExtGetProgramsQCOM(programs, maxPrograms, numPrograms);
	}
#endif // glExtGetProgramsQCOM

#ifdef glExtGetRenderbuffersQCOM
#undef glExtGetRenderbuffersQCOM
	static inline void glExtGetRenderbuffersQCOM(GLuint *renderbuffers, GLint maxRenderbuffers, GLint *numRenderbuffers){
	    glad_debug_glExtGetRenderbuffersQCOM(renderbuffers, maxRenderbuffers, numRenderbuffers);
	}
#endif // glExtGetRenderbuffersQCOM

#ifdef glExtGetShadersQCOM
#undef glExtGetShadersQCOM
	static inline void glExtGetShadersQCOM(GLuint *shaders, GLint maxShaders, GLint *numShaders){
	    glad_debug_glExtGetShadersQCOM(shaders, maxShaders, numShaders);
	}
#endif // glExtGetShadersQCOM

#ifdef glExtGetTexLevelParameterivQCOM
#undef glExtGetTexLevelParameterivQCOM
	static inline void glExtGetTexLevelParameterivQCOM(GLuint texture, GLenum face, GLint level, GLenum pname, GLint *params){
	    glad_debug_glExtGetTexLevelParameterivQCOM(texture, face, level, pname, params);
	}
#endif // glExtGetTexLevelParameterivQCOM

#ifdef glExtGetTexturesQCOM
#undef glExtGetTexturesQCOM
	static inline void glExtGetTexturesQCOM(GLuint *textures, GLint maxTextures, GLint *numTextures){
	    glad_debug_glExtGetTexturesQCOM(textures, maxTextures, numTextures);
	}
#endif // glExtGetTexturesQCOM

#ifdef glExtTexObjectStateOverrideiQCOM
#undef glExtTexObjectStateOverrideiQCOM
	static inline void glExtTexObjectStateOverrideiQCOM(GLenum target, GLenum pname, GLint param){
	    glad_debug_glExtTexObjectStateOverrideiQCOM(target, pname, param);
	}
#endif // glExtTexObjectStateOverrideiQCOM

#ifdef glExtractComponentEXT
#undef glExtractComponentEXT
	static inline void glExtractComponentEXT(GLuint res, GLuint src, GLuint num){
	    glad_debug_glExtractComponentEXT(res, src, num);
	}
#endif // glExtractComponentEXT

#ifdef glFeedbackBuffer
#undef glFeedbackBuffer
	static inline void glFeedbackBuffer(GLsizei size, GLenum type, GLfloat *buffer){
	    glad_debug_glFeedbackBuffer(size, type, buffer);
	}
#endif // glFeedbackBuffer

#ifdef glFeedbackBufferxOES
#undef glFeedbackBufferxOES
	static inline void glFeedbackBufferxOES(GLsizei n, GLenum type, const GLfixed *buffer){
	    glad_debug_glFeedbackBufferxOES(n, type, buffer);
	}
#endif // glFeedbackBufferxOES

#ifdef glFinalCombinerInputNV
#undef glFinalCombinerInputNV
	static inline void glFinalCombinerInputNV(GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage){
	    glad_debug_glFinalCombinerInputNV(variable, input, mapping, componentUsage);
	}
#endif // glFinalCombinerInputNV

#ifdef glFinish
#undef glFinish
	static inline void glFinish(){
	    glad_debug_glFinish();
	}
#endif // glFinish

#ifdef glFinishFenceAPPLE
#undef glFinishFenceAPPLE
	static inline void glFinishFenceAPPLE(GLuint fence){
	    glad_debug_glFinishFenceAPPLE(fence);
	}
#endif // glFinishFenceAPPLE

#ifdef glFinishFenceNV
#undef glFinishFenceNV
	static inline void glFinishFenceNV(GLuint fence){
	    glad_debug_glFinishFenceNV(fence);
	}
#endif // glFinishFenceNV

#ifdef glFinishObjectAPPLE
#undef glFinishObjectAPPLE
	static inline void glFinishObjectAPPLE(GLenum object, GLint name){
	    glad_debug_glFinishObjectAPPLE(object, name);
	}
#endif // glFinishObjectAPPLE

#ifdef glFinishTextureSUNX
#undef glFinishTextureSUNX
	static inline void glFinishTextureSUNX(){
	    glad_debug_glFinishTextureSUNX();
	}
#endif // glFinishTextureSUNX

#ifdef glFlush
#undef glFlush
	static inline void glFlush(){
	    glad_debug_glFlush();
	}
#endif // glFlush

#ifdef glFlushMappedBufferRange
#undef glFlushMappedBufferRange
	static inline void glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length){
	    glad_debug_glFlushMappedBufferRange(target, offset, length);
	}
#endif // glFlushMappedBufferRange

#ifdef glFlushMappedBufferRangeAPPLE
#undef glFlushMappedBufferRangeAPPLE
	static inline void glFlushMappedBufferRangeAPPLE(GLenum target, GLintptr offset, GLsizeiptr size){
	    glad_debug_glFlushMappedBufferRangeAPPLE(target, offset, size);
	}
#endif // glFlushMappedBufferRangeAPPLE

#ifdef glFlushMappedBufferRangeEXT
#undef glFlushMappedBufferRangeEXT
	static inline void glFlushMappedBufferRangeEXT(GLenum target, GLintptr offset, GLsizeiptr length){
	    glad_debug_glFlushMappedBufferRangeEXT(target, offset, length);
	}
#endif // glFlushMappedBufferRangeEXT

#ifdef glFlushMappedNamedBufferRange
#undef glFlushMappedNamedBufferRange
	static inline void glFlushMappedNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length){
	    glad_debug_glFlushMappedNamedBufferRange(buffer, offset, length);
	}
#endif // glFlushMappedNamedBufferRange

#ifdef glFlushMappedNamedBufferRangeEXT
#undef glFlushMappedNamedBufferRangeEXT
	static inline void glFlushMappedNamedBufferRangeEXT(GLuint buffer, GLintptr offset, GLsizeiptr length){
	    glad_debug_glFlushMappedNamedBufferRangeEXT(buffer, offset, length);
	}
#endif // glFlushMappedNamedBufferRangeEXT

#ifdef glFlushPixelDataRangeNV
#undef glFlushPixelDataRangeNV
	static inline void glFlushPixelDataRangeNV(GLenum target){
	    glad_debug_glFlushPixelDataRangeNV(target);
	}
#endif // glFlushPixelDataRangeNV

#ifdef glFlushRasterSGIX
#undef glFlushRasterSGIX
	static inline void glFlushRasterSGIX(){
	    glad_debug_glFlushRasterSGIX();
	}
#endif // glFlushRasterSGIX

#ifdef glFlushStaticDataIBM
#undef glFlushStaticDataIBM
	static inline void glFlushStaticDataIBM(GLenum target){
	    glad_debug_glFlushStaticDataIBM(target);
	}
#endif // glFlushStaticDataIBM

#ifdef glFlushVertexArrayRangeNV
#undef glFlushVertexArrayRangeNV
	static inline void glFlushVertexArrayRangeNV(){
	    glad_debug_glFlushVertexArrayRangeNV();
	}
#endif // glFlushVertexArrayRangeNV

#ifdef glFogCoordFormatNV
#undef glFogCoordFormatNV
	static inline void glFogCoordFormatNV(GLenum type, GLsizei stride){
	    glad_debug_glFogCoordFormatNV(type, stride);
	}
#endif // glFogCoordFormatNV

#ifdef glFogCoordd
#undef glFogCoordd
	static inline void glFogCoordd(GLdouble coord){
	    glad_debug_glFogCoordd(coord);
	}
#endif // glFogCoordd

#ifdef glFogCoorddEXT
#undef glFogCoorddEXT
	static inline void glFogCoorddEXT(GLdouble coord){
	    glad_debug_glFogCoorddEXT(coord);
	}
#endif // glFogCoorddEXT

#ifdef glFogCoorddv
#undef glFogCoorddv
	static inline void glFogCoorddv(const GLdouble *coord){
	    glad_debug_glFogCoorddv(coord);
	}
#endif // glFogCoorddv

#ifdef glFogCoorddvEXT
#undef glFogCoorddvEXT
	static inline void glFogCoorddvEXT(const GLdouble *coord){
	    glad_debug_glFogCoorddvEXT(coord);
	}
#endif // glFogCoorddvEXT

#ifdef glFogCoordf
#undef glFogCoordf
	static inline void glFogCoordf(GLfloat coord){
	    glad_debug_glFogCoordf(coord);
	}
#endif // glFogCoordf

#ifdef glFogCoordfEXT
#undef glFogCoordfEXT
	static inline void glFogCoordfEXT(GLfloat coord){
	    glad_debug_glFogCoordfEXT(coord);
	}
#endif // glFogCoordfEXT

#ifdef glFogCoordfv
#undef glFogCoordfv
	static inline void glFogCoordfv(const GLfloat *coord){
	    glad_debug_glFogCoordfv(coord);
	}
#endif // glFogCoordfv

#ifdef glFogCoordfvEXT
#undef glFogCoordfvEXT
	static inline void glFogCoordfvEXT(const GLfloat *coord){
	    glad_debug_glFogCoordfvEXT(coord);
	}
#endif // glFogCoordfvEXT

#ifdef glFogCoordhNV
#undef glFogCoordhNV
	static inline void glFogCoordhNV(GLhalfNV fog){
	    glad_debug_glFogCoordhNV(fog);
	}
#endif // glFogCoordhNV

#ifdef glFogCoordhvNV
#undef glFogCoordhvNV
	static inline void glFogCoordhvNV(const GLhalfNV *fog){
	    glad_debug_glFogCoordhvNV(fog);
	}
#endif // glFogCoordhvNV

#ifdef glFogFuncSGIS
#undef glFogFuncSGIS
	static inline void glFogFuncSGIS(GLsizei n, const GLfloat *points){
	    glad_debug_glFogFuncSGIS(n, points);
	}
#endif // glFogFuncSGIS

#ifdef glFogf
#undef glFogf
	static inline void glFogf(GLenum pname, GLfloat param){
	    glad_debug_glFogf(pname, param);
	}
#endif // glFogf

#ifdef glFogfv
#undef glFogfv
	static inline void glFogfv(GLenum pname, const GLfloat *params){
	    glad_debug_glFogfv(pname, params);
	}
#endif // glFogfv

#ifdef glFogi
#undef glFogi
	static inline void glFogi(GLenum pname, GLint param){
	    glad_debug_glFogi(pname, param);
	}
#endif // glFogi

#ifdef glFogiv
#undef glFogiv
	static inline void glFogiv(GLenum pname, const GLint *params){
	    glad_debug_glFogiv(pname, params);
	}
#endif // glFogiv

#ifdef glFogx
#undef glFogx
	static inline void glFogx(GLenum pname, GLfixed param){
	    glad_debug_glFogx(pname, param);
	}
#endif // glFogx

#ifdef glFogxOES
#undef glFogxOES
	static inline void glFogxOES(GLenum pname, GLfixed param){
	    glad_debug_glFogxOES(pname, param);
	}
#endif // glFogxOES

#ifdef glFogxv
#undef glFogxv
	static inline void glFogxv(GLenum pname, const GLfixed *param){
	    glad_debug_glFogxv(pname, param);
	}
#endif // glFogxv

#ifdef glFogxvOES
#undef glFogxvOES
	static inline void glFogxvOES(GLenum pname, const GLfixed *param){
	    glad_debug_glFogxvOES(pname, param);
	}
#endif // glFogxvOES

#ifdef glFragmentColorMaterialSGIX
#undef glFragmentColorMaterialSGIX
	static inline void glFragmentColorMaterialSGIX(GLenum face, GLenum mode){
	    glad_debug_glFragmentColorMaterialSGIX(face, mode);
	}
#endif // glFragmentColorMaterialSGIX

#ifdef glFragmentCoverageColorNV
#undef glFragmentCoverageColorNV
	static inline void glFragmentCoverageColorNV(GLuint color){
	    glad_debug_glFragmentCoverageColorNV(color);
	}
#endif // glFragmentCoverageColorNV

#ifdef glFragmentLightModelfSGIX
#undef glFragmentLightModelfSGIX
	static inline void glFragmentLightModelfSGIX(GLenum pname, GLfloat param){
	    glad_debug_glFragmentLightModelfSGIX(pname, param);
	}
#endif // glFragmentLightModelfSGIX

#ifdef glFragmentLightModelfvSGIX
#undef glFragmentLightModelfvSGIX
	static inline void glFragmentLightModelfvSGIX(GLenum pname, const GLfloat *params){
	    glad_debug_glFragmentLightModelfvSGIX(pname, params);
	}
#endif // glFragmentLightModelfvSGIX

#ifdef glFragmentLightModeliSGIX
#undef glFragmentLightModeliSGIX
	static inline void glFragmentLightModeliSGIX(GLenum pname, GLint param){
	    glad_debug_glFragmentLightModeliSGIX(pname, param);
	}
#endif // glFragmentLightModeliSGIX

#ifdef glFragmentLightModelivSGIX
#undef glFragmentLightModelivSGIX
	static inline void glFragmentLightModelivSGIX(GLenum pname, const GLint *params){
	    glad_debug_glFragmentLightModelivSGIX(pname, params);
	}
#endif // glFragmentLightModelivSGIX

#ifdef glFragmentLightfSGIX
#undef glFragmentLightfSGIX
	static inline void glFragmentLightfSGIX(GLenum light, GLenum pname, GLfloat param){
	    glad_debug_glFragmentLightfSGIX(light, pname, param);
	}
#endif // glFragmentLightfSGIX

#ifdef glFragmentLightfvSGIX
#undef glFragmentLightfvSGIX
	static inline void glFragmentLightfvSGIX(GLenum light, GLenum pname, const GLfloat *params){
	    glad_debug_glFragmentLightfvSGIX(light, pname, params);
	}
#endif // glFragmentLightfvSGIX

#ifdef glFragmentLightiSGIX
#undef glFragmentLightiSGIX
	static inline void glFragmentLightiSGIX(GLenum light, GLenum pname, GLint param){
	    glad_debug_glFragmentLightiSGIX(light, pname, param);
	}
#endif // glFragmentLightiSGIX

#ifdef glFragmentLightivSGIX
#undef glFragmentLightivSGIX
	static inline void glFragmentLightivSGIX(GLenum light, GLenum pname, const GLint *params){
	    glad_debug_glFragmentLightivSGIX(light, pname, params);
	}
#endif // glFragmentLightivSGIX

#ifdef glFragmentMaterialfSGIX
#undef glFragmentMaterialfSGIX
	static inline void glFragmentMaterialfSGIX(GLenum face, GLenum pname, GLfloat param){
	    glad_debug_glFragmentMaterialfSGIX(face, pname, param);
	}
#endif // glFragmentMaterialfSGIX

#ifdef glFragmentMaterialfvSGIX
#undef glFragmentMaterialfvSGIX
	static inline void glFragmentMaterialfvSGIX(GLenum face, GLenum pname, const GLfloat *params){
	    glad_debug_glFragmentMaterialfvSGIX(face, pname, params);
	}
#endif // glFragmentMaterialfvSGIX

#ifdef glFragmentMaterialiSGIX
#undef glFragmentMaterialiSGIX
	static inline void glFragmentMaterialiSGIX(GLenum face, GLenum pname, GLint param){
	    glad_debug_glFragmentMaterialiSGIX(face, pname, param);
	}
#endif // glFragmentMaterialiSGIX

#ifdef glFragmentMaterialivSGIX
#undef glFragmentMaterialivSGIX
	static inline void glFragmentMaterialivSGIX(GLenum face, GLenum pname, const GLint *params){
	    glad_debug_glFragmentMaterialivSGIX(face, pname, params);
	}
#endif // glFragmentMaterialivSGIX

#ifdef glFrameTerminatorGREMEDY
#undef glFrameTerminatorGREMEDY
	static inline void glFrameTerminatorGREMEDY(){
	    glad_debug_glFrameTerminatorGREMEDY();
	}
#endif // glFrameTerminatorGREMEDY

#ifdef glFrameZoomSGIX
#undef glFrameZoomSGIX
	static inline void glFrameZoomSGIX(GLint factor){
	    glad_debug_glFrameZoomSGIX(factor);
	}
#endif // glFrameZoomSGIX

#ifdef glFramebufferDrawBufferEXT
#undef glFramebufferDrawBufferEXT
	static inline void glFramebufferDrawBufferEXT(GLuint framebuffer, GLenum mode){
	    glad_debug_glFramebufferDrawBufferEXT(framebuffer, mode);
	}
#endif // glFramebufferDrawBufferEXT

#ifdef glFramebufferDrawBuffersEXT
#undef glFramebufferDrawBuffersEXT
	static inline void glFramebufferDrawBuffersEXT(GLuint framebuffer, GLsizei n, const GLenum *bufs){
	    glad_debug_glFramebufferDrawBuffersEXT(framebuffer, n, bufs);
	}
#endif // glFramebufferDrawBuffersEXT

#ifdef glFramebufferFetchBarrierEXT
#undef glFramebufferFetchBarrierEXT
	static inline void glFramebufferFetchBarrierEXT(){
	    glad_debug_glFramebufferFetchBarrierEXT();
	}
#endif // glFramebufferFetchBarrierEXT

#ifdef glFramebufferFetchBarrierQCOM
#undef glFramebufferFetchBarrierQCOM
	static inline void glFramebufferFetchBarrierQCOM(){
	    glad_debug_glFramebufferFetchBarrierQCOM();
	}
#endif // glFramebufferFetchBarrierQCOM

#ifdef glFramebufferFoveationConfigQCOM
#undef glFramebufferFoveationConfigQCOM
	static inline void glFramebufferFoveationConfigQCOM(GLuint framebuffer, GLuint numLayers, GLuint focalPointsPerLayer, GLuint requestedFeatures, GLuint *providedFeatures){
	    glad_debug_glFramebufferFoveationConfigQCOM(framebuffer, numLayers, focalPointsPerLayer, requestedFeatures, providedFeatures);
	}
#endif // glFramebufferFoveationConfigQCOM

#ifdef glFramebufferFoveationParametersQCOM
#undef glFramebufferFoveationParametersQCOM
	static inline void glFramebufferFoveationParametersQCOM(GLuint framebuffer, GLuint layer, GLuint focalPoint, GLfloat focalX, GLfloat focalY, GLfloat gainX, GLfloat gainY, GLfloat foveaArea){
	    glad_debug_glFramebufferFoveationParametersQCOM(framebuffer, layer, focalPoint, focalX, focalY, gainX, gainY, foveaArea);
	}
#endif // glFramebufferFoveationParametersQCOM

#ifdef glFramebufferParameteri
#undef glFramebufferParameteri
	static inline void glFramebufferParameteri(GLenum target, GLenum pname, GLint param){
	    glad_debug_glFramebufferParameteri(target, pname, param);
	}
#endif // glFramebufferParameteri

#ifdef glFramebufferPixelLocalStorageSizeEXT
#undef glFramebufferPixelLocalStorageSizeEXT
	static inline void glFramebufferPixelLocalStorageSizeEXT(GLuint target, GLsizei size){
	    glad_debug_glFramebufferPixelLocalStorageSizeEXT(target, size);
	}
#endif // glFramebufferPixelLocalStorageSizeEXT

#ifdef glFramebufferReadBufferEXT
#undef glFramebufferReadBufferEXT
	static inline void glFramebufferReadBufferEXT(GLuint framebuffer, GLenum mode){
	    glad_debug_glFramebufferReadBufferEXT(framebuffer, mode);
	}
#endif // glFramebufferReadBufferEXT

#ifdef glFramebufferRenderbuffer
#undef glFramebufferRenderbuffer
	static inline void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer){
	    glad_debug_glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
	}
#endif // glFramebufferRenderbuffer

#ifdef glFramebufferRenderbufferEXT
#undef glFramebufferRenderbufferEXT
	static inline void glFramebufferRenderbufferEXT(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer){
	    glad_debug_glFramebufferRenderbufferEXT(target, attachment, renderbuffertarget, renderbuffer);
	}
#endif // glFramebufferRenderbufferEXT

#ifdef glFramebufferRenderbufferOES
#undef glFramebufferRenderbufferOES
	static inline void glFramebufferRenderbufferOES(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer){
	    glad_debug_glFramebufferRenderbufferOES(target, attachment, renderbuffertarget, renderbuffer);
	}
#endif // glFramebufferRenderbufferOES

#ifdef glFramebufferSampleLocationsfvARB
#undef glFramebufferSampleLocationsfvARB
	static inline void glFramebufferSampleLocationsfvARB(GLenum target, GLuint start, GLsizei count, const GLfloat *v){
	    glad_debug_glFramebufferSampleLocationsfvARB(target, start, count, v);
	}
#endif // glFramebufferSampleLocationsfvARB

#ifdef glFramebufferSampleLocationsfvNV
#undef glFramebufferSampleLocationsfvNV
	static inline void glFramebufferSampleLocationsfvNV(GLenum target, GLuint start, GLsizei count, const GLfloat *v){
	    glad_debug_glFramebufferSampleLocationsfvNV(target, start, count, v);
	}
#endif // glFramebufferSampleLocationsfvNV

#ifdef glFramebufferSamplePositionsfvAMD
#undef glFramebufferSamplePositionsfvAMD
	static inline void glFramebufferSamplePositionsfvAMD(GLenum target, GLuint numsamples, GLuint pixelindex, const GLfloat *values){
	    glad_debug_glFramebufferSamplePositionsfvAMD(target, numsamples, pixelindex, values);
	}
#endif // glFramebufferSamplePositionsfvAMD

#ifdef glFramebufferShadingRateEXT
#undef glFramebufferShadingRateEXT
	static inline void glFramebufferShadingRateEXT(GLenum target, GLenum attachment, GLuint texture, GLint baseLayer, GLsizei numLayers, GLsizei texelWidth, GLsizei texelHeight){
	    glad_debug_glFramebufferShadingRateEXT(target, attachment, texture, baseLayer, numLayers, texelWidth, texelHeight);
	}
#endif // glFramebufferShadingRateEXT

#ifdef glFramebufferTexture
#undef glFramebufferTexture
	static inline void glFramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level){
	    glad_debug_glFramebufferTexture(target, attachment, texture, level);
	}
#endif // glFramebufferTexture

#ifdef glFramebufferTexture1D
#undef glFramebufferTexture1D
	static inline void glFramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level){
	    glad_debug_glFramebufferTexture1D(target, attachment, textarget, texture, level);
	}
#endif // glFramebufferTexture1D

#ifdef glFramebufferTexture1DEXT
#undef glFramebufferTexture1DEXT
	static inline void glFramebufferTexture1DEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level){
	    glad_debug_glFramebufferTexture1DEXT(target, attachment, textarget, texture, level);
	}
#endif // glFramebufferTexture1DEXT

#ifdef glFramebufferTexture2D
#undef glFramebufferTexture2D
	static inline void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level){
	    glad_debug_glFramebufferTexture2D(target, attachment, textarget, texture, level);
	}
#endif // glFramebufferTexture2D

#ifdef glFramebufferTexture2DEXT
#undef glFramebufferTexture2DEXT
	static inline void glFramebufferTexture2DEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level){
	    glad_debug_glFramebufferTexture2DEXT(target, attachment, textarget, texture, level);
	}
#endif // glFramebufferTexture2DEXT

#ifdef glFramebufferTexture2DDownsampleIMG
#undef glFramebufferTexture2DDownsampleIMG
	static inline void glFramebufferTexture2DDownsampleIMG(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint xscale, GLint yscale){
	    glad_debug_glFramebufferTexture2DDownsampleIMG(target, attachment, textarget, texture, level, xscale, yscale);
	}
#endif // glFramebufferTexture2DDownsampleIMG

#ifdef glFramebufferTexture2DMultisampleEXT
#undef glFramebufferTexture2DMultisampleEXT
	static inline void glFramebufferTexture2DMultisampleEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples){
	    glad_debug_glFramebufferTexture2DMultisampleEXT(target, attachment, textarget, texture, level, samples);
	}
#endif // glFramebufferTexture2DMultisampleEXT

#ifdef glFramebufferTexture2DMultisampleIMG
#undef glFramebufferTexture2DMultisampleIMG
	static inline void glFramebufferTexture2DMultisampleIMG(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples){
	    glad_debug_glFramebufferTexture2DMultisampleIMG(target, attachment, textarget, texture, level, samples);
	}
#endif // glFramebufferTexture2DMultisampleIMG

#ifdef glFramebufferTexture2DOES
#undef glFramebufferTexture2DOES
	static inline void glFramebufferTexture2DOES(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level){
	    glad_debug_glFramebufferTexture2DOES(target, attachment, textarget, texture, level);
	}
#endif // glFramebufferTexture2DOES

#ifdef glFramebufferTexture3D
#undef glFramebufferTexture3D
	static inline void glFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset){
	    glad_debug_glFramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
	}
#endif // glFramebufferTexture3D

#ifdef glFramebufferTexture3DEXT
#undef glFramebufferTexture3DEXT
	static inline void glFramebufferTexture3DEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset){
	    glad_debug_glFramebufferTexture3DEXT(target, attachment, textarget, texture, level, zoffset);
	}
#endif // glFramebufferTexture3DEXT

#ifdef glFramebufferTexture3DOES
#undef glFramebufferTexture3DOES
	static inline void glFramebufferTexture3DOES(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset){
	    glad_debug_glFramebufferTexture3DOES(target, attachment, textarget, texture, level, zoffset);
	}
#endif // glFramebufferTexture3DOES

#ifdef glFramebufferTextureARB
#undef glFramebufferTextureARB
	static inline void glFramebufferTextureARB(GLenum target, GLenum attachment, GLuint texture, GLint level){
	    glad_debug_glFramebufferTextureARB(target, attachment, texture, level);
	}
#endif // glFramebufferTextureARB

#ifdef glFramebufferTextureEXT
#undef glFramebufferTextureEXT
	static inline void glFramebufferTextureEXT(GLenum target, GLenum attachment, GLuint texture, GLint level){
	    glad_debug_glFramebufferTextureEXT(target, attachment, texture, level);
	}
#endif // glFramebufferTextureEXT

#ifdef glFramebufferTextureFaceARB
#undef glFramebufferTextureFaceARB
	static inline void glFramebufferTextureFaceARB(GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face){
	    glad_debug_glFramebufferTextureFaceARB(target, attachment, texture, level, face);
	}
#endif // glFramebufferTextureFaceARB

#ifdef glFramebufferTextureFaceEXT
#undef glFramebufferTextureFaceEXT
	static inline void glFramebufferTextureFaceEXT(GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face){
	    glad_debug_glFramebufferTextureFaceEXT(target, attachment, texture, level, face);
	}
#endif // glFramebufferTextureFaceEXT

#ifdef glFramebufferTextureLayer
#undef glFramebufferTextureLayer
	static inline void glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer){
	    glad_debug_glFramebufferTextureLayer(target, attachment, texture, level, layer);
	}
#endif // glFramebufferTextureLayer

#ifdef glFramebufferTextureLayerARB
#undef glFramebufferTextureLayerARB
	static inline void glFramebufferTextureLayerARB(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer){
	    glad_debug_glFramebufferTextureLayerARB(target, attachment, texture, level, layer);
	}
#endif // glFramebufferTextureLayerARB

#ifdef glFramebufferTextureLayerEXT
#undef glFramebufferTextureLayerEXT
	static inline void glFramebufferTextureLayerEXT(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer){
	    glad_debug_glFramebufferTextureLayerEXT(target, attachment, texture, level, layer);
	}
#endif // glFramebufferTextureLayerEXT

#ifdef glFramebufferTextureLayerDownsampleIMG
#undef glFramebufferTextureLayerDownsampleIMG
	static inline void glFramebufferTextureLayerDownsampleIMG(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer, GLint xscale, GLint yscale){
	    glad_debug_glFramebufferTextureLayerDownsampleIMG(target, attachment, texture, level, layer, xscale, yscale);
	}
#endif // glFramebufferTextureLayerDownsampleIMG

#ifdef glFramebufferTextureMultisampleMultiviewOVR
#undef glFramebufferTextureMultisampleMultiviewOVR
	static inline void glFramebufferTextureMultisampleMultiviewOVR(GLenum target, GLenum attachment, GLuint texture, GLint level, GLsizei samples, GLint baseViewIndex, GLsizei numViews){
	    glad_debug_glFramebufferTextureMultisampleMultiviewOVR(target, attachment, texture, level, samples, baseViewIndex, numViews);
	}
#endif // glFramebufferTextureMultisampleMultiviewOVR

#ifdef glFramebufferTextureMultiviewOVR
#undef glFramebufferTextureMultiviewOVR
	static inline void glFramebufferTextureMultiviewOVR(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint baseViewIndex, GLsizei numViews){
	    glad_debug_glFramebufferTextureMultiviewOVR(target, attachment, texture, level, baseViewIndex, numViews);
	}
#endif // glFramebufferTextureMultiviewOVR

#ifdef glFramebufferTextureOES
#undef glFramebufferTextureOES
	static inline void glFramebufferTextureOES(GLenum target, GLenum attachment, GLuint texture, GLint level){
	    glad_debug_glFramebufferTextureOES(target, attachment, texture, level);
	}
#endif // glFramebufferTextureOES

#ifdef glFreeObjectBufferATI
#undef glFreeObjectBufferATI
	static inline void glFreeObjectBufferATI(GLuint buffer){
	    glad_debug_glFreeObjectBufferATI(buffer);
	}
#endif // glFreeObjectBufferATI

#ifdef glFrontFace
#undef glFrontFace
	static inline void glFrontFace(GLenum mode){
	    glad_debug_glFrontFace(mode);
	}
#endif // glFrontFace

#ifdef glFrustum
#undef glFrustum
	static inline void glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar){
	    glad_debug_glFrustum(left, right, bottom, top, zNear, zFar);
	}
#endif // glFrustum

#ifdef glFrustumf
#undef glFrustumf
	static inline void glFrustumf(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f){
	    glad_debug_glFrustumf(l, r, b, t, n, f);
	}
#endif // glFrustumf

#ifdef glFrustumfOES
#undef glFrustumfOES
	static inline void glFrustumfOES(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f){
	    glad_debug_glFrustumfOES(l, r, b, t, n, f);
	}
#endif // glFrustumfOES

#ifdef glFrustumx
#undef glFrustumx
	static inline void glFrustumx(GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f){
	    glad_debug_glFrustumx(l, r, b, t, n, f);
	}
#endif // glFrustumx

#ifdef glFrustumxOES
#undef glFrustumxOES
	static inline void glFrustumxOES(GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f){
	    glad_debug_glFrustumxOES(l, r, b, t, n, f);
	}
#endif // glFrustumxOES

#ifdef glGenBuffers
#undef glGenBuffers
	static inline void glGenBuffers(GLsizei n, GLuint *buffers){
	    glad_debug_glGenBuffers(n, buffers);
	}
#endif // glGenBuffers

#ifdef glGenBuffersARB
#undef glGenBuffersARB
	static inline void glGenBuffersARB(GLsizei n, GLuint *buffers){
	    glad_debug_glGenBuffersARB(n, buffers);
	}
#endif // glGenBuffersARB

#ifdef glGenFencesAPPLE
#undef glGenFencesAPPLE
	static inline void glGenFencesAPPLE(GLsizei n, GLuint *fences){
	    glad_debug_glGenFencesAPPLE(n, fences);
	}
#endif // glGenFencesAPPLE

#ifdef glGenFencesNV
#undef glGenFencesNV
	static inline void glGenFencesNV(GLsizei n, GLuint *fences){
	    glad_debug_glGenFencesNV(n, fences);
	}
#endif // glGenFencesNV

#ifdef glGenFramebuffers
#undef glGenFramebuffers
	static inline void glGenFramebuffers(GLsizei n, GLuint *framebuffers){
	    glad_debug_glGenFramebuffers(n, framebuffers);
	}
#endif // glGenFramebuffers

#ifdef glGenFramebuffersEXT
#undef glGenFramebuffersEXT
	static inline void glGenFramebuffersEXT(GLsizei n, GLuint *framebuffers){
	    glad_debug_glGenFramebuffersEXT(n, framebuffers);
	}
#endif // glGenFramebuffersEXT

#ifdef glGenFramebuffersOES
#undef glGenFramebuffersOES
	static inline void glGenFramebuffersOES(GLsizei n, GLuint *framebuffers){
	    glad_debug_glGenFramebuffersOES(n, framebuffers);
	}
#endif // glGenFramebuffersOES

#ifdef glGenNamesAMD
#undef glGenNamesAMD
	static inline void glGenNamesAMD(GLenum identifier, GLuint num, GLuint *names){
	    glad_debug_glGenNamesAMD(identifier, num, names);
	}
#endif // glGenNamesAMD

#ifdef glGenOcclusionQueriesNV
#undef glGenOcclusionQueriesNV
	static inline void glGenOcclusionQueriesNV(GLsizei n, GLuint *ids){
	    glad_debug_glGenOcclusionQueriesNV(n, ids);
	}
#endif // glGenOcclusionQueriesNV

#ifdef glGenPerfMonitorsAMD
#undef glGenPerfMonitorsAMD
	static inline void glGenPerfMonitorsAMD(GLsizei n, GLuint *monitors){
	    glad_debug_glGenPerfMonitorsAMD(n, monitors);
	}
#endif // glGenPerfMonitorsAMD

#ifdef glGenProgramPipelines
#undef glGenProgramPipelines
	static inline void glGenProgramPipelines(GLsizei n, GLuint *pipelines){
	    glad_debug_glGenProgramPipelines(n, pipelines);
	}
#endif // glGenProgramPipelines

#ifdef glGenProgramPipelinesEXT
#undef glGenProgramPipelinesEXT
	static inline void glGenProgramPipelinesEXT(GLsizei n, GLuint *pipelines){
	    glad_debug_glGenProgramPipelinesEXT(n, pipelines);
	}
#endif // glGenProgramPipelinesEXT

#ifdef glGenProgramsARB
#undef glGenProgramsARB
	static inline void glGenProgramsARB(GLsizei n, GLuint *programs){
	    glad_debug_glGenProgramsARB(n, programs);
	}
#endif // glGenProgramsARB

#ifdef glGenProgramsNV
#undef glGenProgramsNV
	static inline void glGenProgramsNV(GLsizei n, GLuint *programs){
	    glad_debug_glGenProgramsNV(n, programs);
	}
#endif // glGenProgramsNV

#ifdef glGenQueries
#undef glGenQueries
	static inline void glGenQueries(GLsizei n, GLuint *ids){
	    glad_debug_glGenQueries(n, ids);
	}
#endif // glGenQueries

#ifdef glGenQueriesARB
#undef glGenQueriesARB
	static inline void glGenQueriesARB(GLsizei n, GLuint *ids){
	    glad_debug_glGenQueriesARB(n, ids);
	}
#endif // glGenQueriesARB

#ifdef glGenQueriesEXT
#undef glGenQueriesEXT
	static inline void glGenQueriesEXT(GLsizei n, GLuint *ids){
	    glad_debug_glGenQueriesEXT(n, ids);
	}
#endif // glGenQueriesEXT

#ifdef glGenQueryResourceTagNV
#undef glGenQueryResourceTagNV
	static inline void glGenQueryResourceTagNV(GLsizei n, GLint *tagIds){
	    glad_debug_glGenQueryResourceTagNV(n, tagIds);
	}
#endif // glGenQueryResourceTagNV

#ifdef glGenRenderbuffers
#undef glGenRenderbuffers
	static inline void glGenRenderbuffers(GLsizei n, GLuint *renderbuffers){
	    glad_debug_glGenRenderbuffers(n, renderbuffers);
	}
#endif // glGenRenderbuffers

#ifdef glGenRenderbuffersEXT
#undef glGenRenderbuffersEXT
	static inline void glGenRenderbuffersEXT(GLsizei n, GLuint *renderbuffers){
	    glad_debug_glGenRenderbuffersEXT(n, renderbuffers);
	}
#endif // glGenRenderbuffersEXT

#ifdef glGenRenderbuffersOES
#undef glGenRenderbuffersOES
	static inline void glGenRenderbuffersOES(GLsizei n, GLuint *renderbuffers){
	    glad_debug_glGenRenderbuffersOES(n, renderbuffers);
	}
#endif // glGenRenderbuffersOES

#ifdef glGenSamplers
#undef glGenSamplers
	static inline void glGenSamplers(GLsizei count, GLuint *samplers){
	    glad_debug_glGenSamplers(count, samplers);
	}
#endif // glGenSamplers

#ifdef glGenSemaphoresEXT
#undef glGenSemaphoresEXT
	static inline void glGenSemaphoresEXT(GLsizei n, GLuint *semaphores){
	    glad_debug_glGenSemaphoresEXT(n, semaphores);
	}
#endif // glGenSemaphoresEXT

#ifdef glGenTextures
#undef glGenTextures
	static inline void glGenTextures(GLsizei n, GLuint *textures){
	    glad_debug_glGenTextures(n, textures);
	}
#endif // glGenTextures

#ifdef glGenTexturesEXT
#undef glGenTexturesEXT
	static inline void glGenTexturesEXT(GLsizei n, GLuint *textures){
	    glad_debug_glGenTexturesEXT(n, textures);
	}
#endif // glGenTexturesEXT

#ifdef glGenTransformFeedbacks
#undef glGenTransformFeedbacks
	static inline void glGenTransformFeedbacks(GLsizei n, GLuint *ids){
	    glad_debug_glGenTransformFeedbacks(n, ids);
	}
#endif // glGenTransformFeedbacks

#ifdef glGenTransformFeedbacksNV
#undef glGenTransformFeedbacksNV
	static inline void glGenTransformFeedbacksNV(GLsizei n, GLuint *ids){
	    glad_debug_glGenTransformFeedbacksNV(n, ids);
	}
#endif // glGenTransformFeedbacksNV

#ifdef glGenVertexArrays
#undef glGenVertexArrays
	static inline void glGenVertexArrays(GLsizei n, GLuint *arrays){
	    glad_debug_glGenVertexArrays(n, arrays);
	}
#endif // glGenVertexArrays

#ifdef glGenVertexArraysAPPLE
#undef glGenVertexArraysAPPLE
	static inline void glGenVertexArraysAPPLE(GLsizei n, GLuint *arrays){
	    glad_debug_glGenVertexArraysAPPLE(n, arrays);
	}
#endif // glGenVertexArraysAPPLE

#ifdef glGenVertexArraysOES
#undef glGenVertexArraysOES
	static inline void glGenVertexArraysOES(GLsizei n, GLuint *arrays){
	    glad_debug_glGenVertexArraysOES(n, arrays);
	}
#endif // glGenVertexArraysOES

#ifdef glGenerateMipmap
#undef glGenerateMipmap
	static inline void glGenerateMipmap(GLenum target){
	    glad_debug_glGenerateMipmap(target);
	}
#endif // glGenerateMipmap

#ifdef glGenerateMipmapEXT
#undef glGenerateMipmapEXT
	static inline void glGenerateMipmapEXT(GLenum target){
	    glad_debug_glGenerateMipmapEXT(target);
	}
#endif // glGenerateMipmapEXT

#ifdef glGenerateMipmapOES
#undef glGenerateMipmapOES
	static inline void glGenerateMipmapOES(GLenum target){
	    glad_debug_glGenerateMipmapOES(target);
	}
#endif // glGenerateMipmapOES

#ifdef glGenerateMultiTexMipmapEXT
#undef glGenerateMultiTexMipmapEXT
	static inline void glGenerateMultiTexMipmapEXT(GLenum texunit, GLenum target){
	    glad_debug_glGenerateMultiTexMipmapEXT(texunit, target);
	}
#endif // glGenerateMultiTexMipmapEXT

#ifdef glGenerateTextureMipmap
#undef glGenerateTextureMipmap
	static inline void glGenerateTextureMipmap(GLuint texture){
	    glad_debug_glGenerateTextureMipmap(texture);
	}
#endif // glGenerateTextureMipmap

#ifdef glGenerateTextureMipmapEXT
#undef glGenerateTextureMipmapEXT
	static inline void glGenerateTextureMipmapEXT(GLuint texture, GLenum target){
	    glad_debug_glGenerateTextureMipmapEXT(texture, target);
	}
#endif // glGenerateTextureMipmapEXT

#ifdef glGetActiveAtomicCounterBufferiv
#undef glGetActiveAtomicCounterBufferiv
	static inline void glGetActiveAtomicCounterBufferiv(GLuint program, GLuint bufferIndex, GLenum pname, GLint *params){
	    glad_debug_glGetActiveAtomicCounterBufferiv(program, bufferIndex, pname, params);
	}
#endif // glGetActiveAtomicCounterBufferiv

#ifdef glGetActiveAttrib
#undef glGetActiveAttrib
	static inline void glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name){
	    glad_debug_glGetActiveAttrib(program, index, bufSize, length, size, type, name);
	}
#endif // glGetActiveAttrib

#ifdef glGetActiveAttribARB
#undef glGetActiveAttribARB
	static inline void glGetActiveAttribARB(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name){
	    glad_debug_glGetActiveAttribARB(programObj, index, maxLength, length, size, type, name);
	}
#endif // glGetActiveAttribARB

#ifdef glGetActiveSubroutineName
#undef glGetActiveSubroutineName
	static inline void glGetActiveSubroutineName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name){
	    glad_debug_glGetActiveSubroutineName(program, shadertype, index, bufSize, length, name);
	}
#endif // glGetActiveSubroutineName

#ifdef glGetActiveSubroutineUniformName
#undef glGetActiveSubroutineUniformName
	static inline void glGetActiveSubroutineUniformName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name){
	    glad_debug_glGetActiveSubroutineUniformName(program, shadertype, index, bufSize, length, name);
	}
#endif // glGetActiveSubroutineUniformName

#ifdef glGetActiveSubroutineUniformiv
#undef glGetActiveSubroutineUniformiv
	static inline void glGetActiveSubroutineUniformiv(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values){
	    glad_debug_glGetActiveSubroutineUniformiv(program, shadertype, index, pname, values);
	}
#endif // glGetActiveSubroutineUniformiv

#ifdef glGetActiveUniform
#undef glGetActiveUniform
	static inline void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name){
	    glad_debug_glGetActiveUniform(program, index, bufSize, length, size, type, name);
	}
#endif // glGetActiveUniform

#ifdef glGetActiveUniformARB
#undef glGetActiveUniformARB
	static inline void glGetActiveUniformARB(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name){
	    glad_debug_glGetActiveUniformARB(programObj, index, maxLength, length, size, type, name);
	}
#endif // glGetActiveUniformARB

#ifdef glGetActiveUniformBlockName
#undef glGetActiveUniformBlockName
	static inline void glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName){
	    glad_debug_glGetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName);
	}
#endif // glGetActiveUniformBlockName

#ifdef glGetActiveUniformBlockiv
#undef glGetActiveUniformBlockiv
	static inline void glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params){
	    glad_debug_glGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
	}
#endif // glGetActiveUniformBlockiv

#ifdef glGetActiveUniformName
#undef glGetActiveUniformName
	static inline void glGetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName){
	    glad_debug_glGetActiveUniformName(program, uniformIndex, bufSize, length, uniformName);
	}
#endif // glGetActiveUniformName

#ifdef glGetActiveUniformsiv
#undef glGetActiveUniformsiv
	static inline void glGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params){
	    glad_debug_glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
	}
#endif // glGetActiveUniformsiv

#ifdef glGetActiveVaryingNV
#undef glGetActiveVaryingNV
	static inline void glGetActiveVaryingNV(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name){
	    glad_debug_glGetActiveVaryingNV(program, index, bufSize, length, size, type, name);
	}
#endif // glGetActiveVaryingNV

#ifdef glGetArrayObjectfvATI
#undef glGetArrayObjectfvATI
	static inline void glGetArrayObjectfvATI(GLenum array, GLenum pname, GLfloat *params){
	    glad_debug_glGetArrayObjectfvATI(array, pname, params);
	}
#endif // glGetArrayObjectfvATI

#ifdef glGetArrayObjectivATI
#undef glGetArrayObjectivATI
	static inline void glGetArrayObjectivATI(GLenum array, GLenum pname, GLint *params){
	    glad_debug_glGetArrayObjectivATI(array, pname, params);
	}
#endif // glGetArrayObjectivATI

#ifdef glGetAttachedObjectsARB
#undef glGetAttachedObjectsARB
	static inline void glGetAttachedObjectsARB(GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj){
	    glad_debug_glGetAttachedObjectsARB(containerObj, maxCount, count, obj);
	}
#endif // glGetAttachedObjectsARB

#ifdef glGetAttachedShaders
#undef glGetAttachedShaders
	static inline void glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders){
	    glad_debug_glGetAttachedShaders(program, maxCount, count, shaders);
	}
#endif // glGetAttachedShaders

#ifdef glGetBooleanIndexedvEXT
#undef glGetBooleanIndexedvEXT
	static inline void glGetBooleanIndexedvEXT(GLenum target, GLuint index, GLboolean *data){
	    glad_debug_glGetBooleanIndexedvEXT(target, index, data);
	}
#endif // glGetBooleanIndexedvEXT

#ifdef glGetBooleani_v
#undef glGetBooleani_v
	static inline void glGetBooleani_v(GLenum target, GLuint index, GLboolean *data){
	    glad_debug_glGetBooleani_v(target, index, data);
	}
#endif // glGetBooleani_v

#ifdef glGetBooleanv
#undef glGetBooleanv
	static inline void glGetBooleanv(GLenum pname, GLboolean *data){
	    glad_debug_glGetBooleanv(pname, data);
	}
#endif // glGetBooleanv

#ifdef glGetBufferParameteri64v
#undef glGetBufferParameteri64v
	static inline void glGetBufferParameteri64v(GLenum target, GLenum pname, GLint64 *params){
	    glad_debug_glGetBufferParameteri64v(target, pname, params);
	}
#endif // glGetBufferParameteri64v

#ifdef glGetBufferParameteriv
#undef glGetBufferParameteriv
	static inline void glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetBufferParameteriv(target, pname, params);
	}
#endif // glGetBufferParameteriv

#ifdef glGetBufferParameterivARB
#undef glGetBufferParameterivARB
	static inline void glGetBufferParameterivARB(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetBufferParameterivARB(target, pname, params);
	}
#endif // glGetBufferParameterivARB

#ifdef glGetBufferParameterui64vNV
#undef glGetBufferParameterui64vNV
	static inline void glGetBufferParameterui64vNV(GLenum target, GLenum pname, GLuint64EXT *params){
	    glad_debug_glGetBufferParameterui64vNV(target, pname, params);
	}
#endif // glGetBufferParameterui64vNV

#ifdef glGetClipPlane
#undef glGetClipPlane
	static inline void glGetClipPlane(GLenum plane, GLdouble *equation){
	    glad_debug_glGetClipPlane(plane, equation);
	}
#endif // glGetClipPlane

#ifdef glGetClipPlanef
#undef glGetClipPlanef
	static inline void glGetClipPlanef(GLenum plane, GLfloat *equation){
	    glad_debug_glGetClipPlanef(plane, equation);
	}
#endif // glGetClipPlanef

#ifdef glGetClipPlanefOES
#undef glGetClipPlanefOES
	static inline void glGetClipPlanefOES(GLenum plane, GLfloat *equation){
	    glad_debug_glGetClipPlanefOES(plane, equation);
	}
#endif // glGetClipPlanefOES

#ifdef glGetClipPlanex
#undef glGetClipPlanex
	static inline void glGetClipPlanex(GLenum plane, GLfixed *equation){
	    glad_debug_glGetClipPlanex(plane, equation);
	}
#endif // glGetClipPlanex

#ifdef glGetClipPlanexOES
#undef glGetClipPlanexOES
	static inline void glGetClipPlanexOES(GLenum plane, GLfixed *equation){
	    glad_debug_glGetClipPlanexOES(plane, equation);
	}
#endif // glGetClipPlanexOES

#ifdef glGetColorTableParameterfv
#undef glGetColorTableParameterfv
	static inline void glGetColorTableParameterfv(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetColorTableParameterfv(target, pname, params);
	}
#endif // glGetColorTableParameterfv

#ifdef glGetColorTableParameterfvEXT
#undef glGetColorTableParameterfvEXT
	static inline void glGetColorTableParameterfvEXT(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetColorTableParameterfvEXT(target, pname, params);
	}
#endif // glGetColorTableParameterfvEXT

#ifdef glGetColorTableParameterfvSGI
#undef glGetColorTableParameterfvSGI
	static inline void glGetColorTableParameterfvSGI(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetColorTableParameterfvSGI(target, pname, params);
	}
#endif // glGetColorTableParameterfvSGI

#ifdef glGetColorTableParameteriv
#undef glGetColorTableParameteriv
	static inline void glGetColorTableParameteriv(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetColorTableParameteriv(target, pname, params);
	}
#endif // glGetColorTableParameteriv

#ifdef glGetColorTableParameterivEXT
#undef glGetColorTableParameterivEXT
	static inline void glGetColorTableParameterivEXT(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetColorTableParameterivEXT(target, pname, params);
	}
#endif // glGetColorTableParameterivEXT

#ifdef glGetColorTableParameterivSGI
#undef glGetColorTableParameterivSGI
	static inline void glGetColorTableParameterivSGI(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetColorTableParameterivSGI(target, pname, params);
	}
#endif // glGetColorTableParameterivSGI

#ifdef glGetCombinerInputParameterfvNV
#undef glGetCombinerInputParameterfvNV
	static inline void glGetCombinerInputParameterfvNV(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat *params){
	    glad_debug_glGetCombinerInputParameterfvNV(stage, portion, variable, pname, params);
	}
#endif // glGetCombinerInputParameterfvNV

#ifdef glGetCombinerInputParameterivNV
#undef glGetCombinerInputParameterivNV
	static inline void glGetCombinerInputParameterivNV(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint *params){
	    glad_debug_glGetCombinerInputParameterivNV(stage, portion, variable, pname, params);
	}
#endif // glGetCombinerInputParameterivNV

#ifdef glGetCombinerOutputParameterfvNV
#undef glGetCombinerOutputParameterfvNV
	static inline void glGetCombinerOutputParameterfvNV(GLenum stage, GLenum portion, GLenum pname, GLfloat *params){
	    glad_debug_glGetCombinerOutputParameterfvNV(stage, portion, pname, params);
	}
#endif // glGetCombinerOutputParameterfvNV

#ifdef glGetCombinerOutputParameterivNV
#undef glGetCombinerOutputParameterivNV
	static inline void glGetCombinerOutputParameterivNV(GLenum stage, GLenum portion, GLenum pname, GLint *params){
	    glad_debug_glGetCombinerOutputParameterivNV(stage, portion, pname, params);
	}
#endif // glGetCombinerOutputParameterivNV

#ifdef glGetCombinerStageParameterfvNV
#undef glGetCombinerStageParameterfvNV
	static inline void glGetCombinerStageParameterfvNV(GLenum stage, GLenum pname, GLfloat *params){
	    glad_debug_glGetCombinerStageParameterfvNV(stage, pname, params);
	}
#endif // glGetCombinerStageParameterfvNV

#ifdef glGetConvolutionParameterfv
#undef glGetConvolutionParameterfv
	static inline void glGetConvolutionParameterfv(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetConvolutionParameterfv(target, pname, params);
	}
#endif // glGetConvolutionParameterfv

#ifdef glGetConvolutionParameterfvEXT
#undef glGetConvolutionParameterfvEXT
	static inline void glGetConvolutionParameterfvEXT(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetConvolutionParameterfvEXT(target, pname, params);
	}
#endif // glGetConvolutionParameterfvEXT

#ifdef glGetConvolutionParameteriv
#undef glGetConvolutionParameteriv
	static inline void glGetConvolutionParameteriv(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetConvolutionParameteriv(target, pname, params);
	}
#endif // glGetConvolutionParameteriv

#ifdef glGetConvolutionParameterivEXT
#undef glGetConvolutionParameterivEXT
	static inline void glGetConvolutionParameterivEXT(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetConvolutionParameterivEXT(target, pname, params);
	}
#endif // glGetConvolutionParameterivEXT

#ifdef glGetConvolutionParameterxvOES
#undef glGetConvolutionParameterxvOES
	static inline void glGetConvolutionParameterxvOES(GLenum target, GLenum pname, GLfixed *params){
	    glad_debug_glGetConvolutionParameterxvOES(target, pname, params);
	}
#endif // glGetConvolutionParameterxvOES

#ifdef glGetCoverageModulationTableNV
#undef glGetCoverageModulationTableNV
	static inline void glGetCoverageModulationTableNV(GLsizei bufSize, GLfloat *v){
	    glad_debug_glGetCoverageModulationTableNV(bufSize, v);
	}
#endif // glGetCoverageModulationTableNV

#ifdef glGetDetailTexFuncSGIS
#undef glGetDetailTexFuncSGIS
	static inline void glGetDetailTexFuncSGIS(GLenum target, GLfloat *points){
	    glad_debug_glGetDetailTexFuncSGIS(target, points);
	}
#endif // glGetDetailTexFuncSGIS

#ifdef glGetDoubleIndexedvEXT
#undef glGetDoubleIndexedvEXT
	static inline void glGetDoubleIndexedvEXT(GLenum target, GLuint index, GLdouble *data){
	    glad_debug_glGetDoubleIndexedvEXT(target, index, data);
	}
#endif // glGetDoubleIndexedvEXT

#ifdef glGetDoublei_v
#undef glGetDoublei_v
	static inline void glGetDoublei_v(GLenum target, GLuint index, GLdouble *data){
	    glad_debug_glGetDoublei_v(target, index, data);
	}
#endif // glGetDoublei_v

#ifdef glGetDoublei_vEXT
#undef glGetDoublei_vEXT
	static inline void glGetDoublei_vEXT(GLenum pname, GLuint index, GLdouble *params){
	    glad_debug_glGetDoublei_vEXT(pname, index, params);
	}
#endif // glGetDoublei_vEXT

#ifdef glGetDoublev
#undef glGetDoublev
	static inline void glGetDoublev(GLenum pname, GLdouble *data){
	    glad_debug_glGetDoublev(pname, data);
	}
#endif // glGetDoublev

#ifdef glGetDriverControlStringQCOM
#undef glGetDriverControlStringQCOM
	static inline void glGetDriverControlStringQCOM(GLuint driverControl, GLsizei bufSize, GLsizei *length, GLchar *driverControlString){
	    glad_debug_glGetDriverControlStringQCOM(driverControl, bufSize, length, driverControlString);
	}
#endif // glGetDriverControlStringQCOM

#ifdef glGetDriverControlsQCOM
#undef glGetDriverControlsQCOM
	static inline void glGetDriverControlsQCOM(GLint *num, GLsizei size, GLuint *driverControls){
	    glad_debug_glGetDriverControlsQCOM(num, size, driverControls);
	}
#endif // glGetDriverControlsQCOM

#ifdef glGetFenceivNV
#undef glGetFenceivNV
	static inline void glGetFenceivNV(GLuint fence, GLenum pname, GLint *params){
	    glad_debug_glGetFenceivNV(fence, pname, params);
	}
#endif // glGetFenceivNV

#ifdef glGetFinalCombinerInputParameterfvNV
#undef glGetFinalCombinerInputParameterfvNV
	static inline void glGetFinalCombinerInputParameterfvNV(GLenum variable, GLenum pname, GLfloat *params){
	    glad_debug_glGetFinalCombinerInputParameterfvNV(variable, pname, params);
	}
#endif // glGetFinalCombinerInputParameterfvNV

#ifdef glGetFinalCombinerInputParameterivNV
#undef glGetFinalCombinerInputParameterivNV
	static inline void glGetFinalCombinerInputParameterivNV(GLenum variable, GLenum pname, GLint *params){
	    glad_debug_glGetFinalCombinerInputParameterivNV(variable, pname, params);
	}
#endif // glGetFinalCombinerInputParameterivNV

#ifdef glGetFirstPerfQueryIdINTEL
#undef glGetFirstPerfQueryIdINTEL
	static inline void glGetFirstPerfQueryIdINTEL(GLuint *queryId){
	    glad_debug_glGetFirstPerfQueryIdINTEL(queryId);
	}
#endif // glGetFirstPerfQueryIdINTEL

#ifdef glGetFixedv
#undef glGetFixedv
	static inline void glGetFixedv(GLenum pname, GLfixed *data){
	    glad_debug_glGetFixedv(pname, data);
	}
#endif // glGetFixedv

#ifdef glGetFixedvOES
#undef glGetFixedvOES
	static inline void glGetFixedvOES(GLenum pname, GLfixed *params){
	    glad_debug_glGetFixedvOES(pname, params);
	}
#endif // glGetFixedvOES

#ifdef glGetFloatIndexedvEXT
#undef glGetFloatIndexedvEXT
	static inline void glGetFloatIndexedvEXT(GLenum target, GLuint index, GLfloat *data){
	    glad_debug_glGetFloatIndexedvEXT(target, index, data);
	}
#endif // glGetFloatIndexedvEXT

#ifdef glGetFloati_v
#undef glGetFloati_v
	static inline void glGetFloati_v(GLenum target, GLuint index, GLfloat *data){
	    glad_debug_glGetFloati_v(target, index, data);
	}
#endif // glGetFloati_v

#ifdef glGetFloati_vEXT
#undef glGetFloati_vEXT
	static inline void glGetFloati_vEXT(GLenum pname, GLuint index, GLfloat *params){
	    glad_debug_glGetFloati_vEXT(pname, index, params);
	}
#endif // glGetFloati_vEXT

#ifdef glGetFloati_vNV
#undef glGetFloati_vNV
	static inline void glGetFloati_vNV(GLenum target, GLuint index, GLfloat *data){
	    glad_debug_glGetFloati_vNV(target, index, data);
	}
#endif // glGetFloati_vNV

#ifdef glGetFloati_vOES
#undef glGetFloati_vOES
	static inline void glGetFloati_vOES(GLenum target, GLuint index, GLfloat *data){
	    glad_debug_glGetFloati_vOES(target, index, data);
	}
#endif // glGetFloati_vOES

#ifdef glGetFloatv
#undef glGetFloatv
	static inline void glGetFloatv(GLenum pname, GLfloat *data){
	    glad_debug_glGetFloatv(pname, data);
	}
#endif // glGetFloatv

#ifdef glGetFogFuncSGIS
#undef glGetFogFuncSGIS
	static inline void glGetFogFuncSGIS(GLfloat *points){
	    glad_debug_glGetFogFuncSGIS(points);
	}
#endif // glGetFogFuncSGIS

#ifdef glGetFragmentLightfvSGIX
#undef glGetFragmentLightfvSGIX
	static inline void glGetFragmentLightfvSGIX(GLenum light, GLenum pname, GLfloat *params){
	    glad_debug_glGetFragmentLightfvSGIX(light, pname, params);
	}
#endif // glGetFragmentLightfvSGIX

#ifdef glGetFragmentLightivSGIX
#undef glGetFragmentLightivSGIX
	static inline void glGetFragmentLightivSGIX(GLenum light, GLenum pname, GLint *params){
	    glad_debug_glGetFragmentLightivSGIX(light, pname, params);
	}
#endif // glGetFragmentLightivSGIX

#ifdef glGetFragmentMaterialfvSGIX
#undef glGetFragmentMaterialfvSGIX
	static inline void glGetFragmentMaterialfvSGIX(GLenum face, GLenum pname, GLfloat *params){
	    glad_debug_glGetFragmentMaterialfvSGIX(face, pname, params);
	}
#endif // glGetFragmentMaterialfvSGIX

#ifdef glGetFragmentMaterialivSGIX
#undef glGetFragmentMaterialivSGIX
	static inline void glGetFragmentMaterialivSGIX(GLenum face, GLenum pname, GLint *params){
	    glad_debug_glGetFragmentMaterialivSGIX(face, pname, params);
	}
#endif // glGetFragmentMaterialivSGIX

#ifdef glGetFragmentShadingRatesEXT
#undef glGetFragmentShadingRatesEXT
	static inline void glGetFragmentShadingRatesEXT(GLsizei samples, GLsizei maxCount, GLsizei *count, GLenum *shadingRates){
	    glad_debug_glGetFragmentShadingRatesEXT(samples, maxCount, count, shadingRates);
	}
#endif // glGetFragmentShadingRatesEXT

#ifdef glGetFramebufferAttachmentParameteriv
#undef glGetFramebufferAttachmentParameteriv
	static inline void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params){
	    glad_debug_glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
	}
#endif // glGetFramebufferAttachmentParameteriv

#ifdef glGetFramebufferAttachmentParameterivEXT
#undef glGetFramebufferAttachmentParameterivEXT
	static inline void glGetFramebufferAttachmentParameterivEXT(GLenum target, GLenum attachment, GLenum pname, GLint *params){
	    glad_debug_glGetFramebufferAttachmentParameterivEXT(target, attachment, pname, params);
	}
#endif // glGetFramebufferAttachmentParameterivEXT

#ifdef glGetFramebufferAttachmentParameterivOES
#undef glGetFramebufferAttachmentParameterivOES
	static inline void glGetFramebufferAttachmentParameterivOES(GLenum target, GLenum attachment, GLenum pname, GLint *params){
	    glad_debug_glGetFramebufferAttachmentParameterivOES(target, attachment, pname, params);
	}
#endif // glGetFramebufferAttachmentParameterivOES

#ifdef glGetFramebufferParameterfvAMD
#undef glGetFramebufferParameterfvAMD
	static inline void glGetFramebufferParameterfvAMD(GLenum target, GLenum pname, GLuint numsamples, GLuint pixelindex, GLsizei size, GLfloat *values){
	    glad_debug_glGetFramebufferParameterfvAMD(target, pname, numsamples, pixelindex, size, values);
	}
#endif // glGetFramebufferParameterfvAMD

#ifdef glGetFramebufferParameteriv
#undef glGetFramebufferParameteriv
	static inline void glGetFramebufferParameteriv(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetFramebufferParameteriv(target, pname, params);
	}
#endif // glGetFramebufferParameteriv

#ifdef glGetFramebufferParameterivEXT
#undef glGetFramebufferParameterivEXT
	static inline void glGetFramebufferParameterivEXT(GLuint framebuffer, GLenum pname, GLint *params){
	    glad_debug_glGetFramebufferParameterivEXT(framebuffer, pname, params);
	}
#endif // glGetFramebufferParameterivEXT

#ifdef glGetHistogramParameterfv
#undef glGetHistogramParameterfv
	static inline void glGetHistogramParameterfv(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetHistogramParameterfv(target, pname, params);
	}
#endif // glGetHistogramParameterfv

#ifdef glGetHistogramParameterfvEXT
#undef glGetHistogramParameterfvEXT
	static inline void glGetHistogramParameterfvEXT(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetHistogramParameterfvEXT(target, pname, params);
	}
#endif // glGetHistogramParameterfvEXT

#ifdef glGetHistogramParameteriv
#undef glGetHistogramParameteriv
	static inline void glGetHistogramParameteriv(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetHistogramParameteriv(target, pname, params);
	}
#endif // glGetHistogramParameteriv

#ifdef glGetHistogramParameterivEXT
#undef glGetHistogramParameterivEXT
	static inline void glGetHistogramParameterivEXT(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetHistogramParameterivEXT(target, pname, params);
	}
#endif // glGetHistogramParameterivEXT

#ifdef glGetHistogramParameterxvOES
#undef glGetHistogramParameterxvOES
	static inline void glGetHistogramParameterxvOES(GLenum target, GLenum pname, GLfixed *params){
	    glad_debug_glGetHistogramParameterxvOES(target, pname, params);
	}
#endif // glGetHistogramParameterxvOES

#ifdef glGetImageTransformParameterfvHP
#undef glGetImageTransformParameterfvHP
	static inline void glGetImageTransformParameterfvHP(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetImageTransformParameterfvHP(target, pname, params);
	}
#endif // glGetImageTransformParameterfvHP

#ifdef glGetImageTransformParameterivHP
#undef glGetImageTransformParameterivHP
	static inline void glGetImageTransformParameterivHP(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetImageTransformParameterivHP(target, pname, params);
	}
#endif // glGetImageTransformParameterivHP

#ifdef glGetInfoLogARB
#undef glGetInfoLogARB
	static inline void glGetInfoLogARB(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog){
	    glad_debug_glGetInfoLogARB(obj, maxLength, length, infoLog);
	}
#endif // glGetInfoLogARB

#ifdef glGetInteger64i_v
#undef glGetInteger64i_v
	static inline void glGetInteger64i_v(GLenum target, GLuint index, GLint64 *data){
	    glad_debug_glGetInteger64i_v(target, index, data);
	}
#endif // glGetInteger64i_v

#ifdef glGetInteger64v
#undef glGetInteger64v
	static inline void glGetInteger64v(GLenum pname, GLint64 *data){
	    glad_debug_glGetInteger64v(pname, data);
	}
#endif // glGetInteger64v

#ifdef glGetInteger64vAPPLE
#undef glGetInteger64vAPPLE
	static inline void glGetInteger64vAPPLE(GLenum pname, GLint64 *params){
	    glad_debug_glGetInteger64vAPPLE(pname, params);
	}
#endif // glGetInteger64vAPPLE

#ifdef glGetInteger64vEXT
#undef glGetInteger64vEXT
	static inline void glGetInteger64vEXT(GLenum pname, GLint64 *data){
	    glad_debug_glGetInteger64vEXT(pname, data);
	}
#endif // glGetInteger64vEXT

#ifdef glGetIntegerIndexedvEXT
#undef glGetIntegerIndexedvEXT
	static inline void glGetIntegerIndexedvEXT(GLenum target, GLuint index, GLint *data){
	    glad_debug_glGetIntegerIndexedvEXT(target, index, data);
	}
#endif // glGetIntegerIndexedvEXT

#ifdef glGetIntegeri_v
#undef glGetIntegeri_v
	static inline void glGetIntegeri_v(GLenum target, GLuint index, GLint *data){
	    glad_debug_glGetIntegeri_v(target, index, data);
	}
#endif // glGetIntegeri_v

#ifdef glGetIntegeri_vEXT
#undef glGetIntegeri_vEXT
	static inline void glGetIntegeri_vEXT(GLenum target, GLuint index, GLint *data){
	    glad_debug_glGetIntegeri_vEXT(target, index, data);
	}
#endif // glGetIntegeri_vEXT

#ifdef glGetIntegerui64i_vNV
#undef glGetIntegerui64i_vNV
	static inline void glGetIntegerui64i_vNV(GLenum value, GLuint index, GLuint64EXT *result){
	    glad_debug_glGetIntegerui64i_vNV(value, index, result);
	}
#endif // glGetIntegerui64i_vNV

#ifdef glGetIntegerui64vNV
#undef glGetIntegerui64vNV
	static inline void glGetIntegerui64vNV(GLenum value, GLuint64EXT *result){
	    glad_debug_glGetIntegerui64vNV(value, result);
	}
#endif // glGetIntegerui64vNV

#ifdef glGetIntegerv
#undef glGetIntegerv
	static inline void glGetIntegerv(GLenum pname, GLint *data){
	    glad_debug_glGetIntegerv(pname, data);
	}
#endif // glGetIntegerv

#ifdef glGetInternalformatSampleivNV
#undef glGetInternalformatSampleivNV
	static inline void glGetInternalformatSampleivNV(GLenum target, GLenum internalformat, GLsizei samples, GLenum pname, GLsizei count, GLint *params){
	    glad_debug_glGetInternalformatSampleivNV(target, internalformat, samples, pname, count, params);
	}
#endif // glGetInternalformatSampleivNV

#ifdef glGetInternalformati64v
#undef glGetInternalformati64v
	static inline void glGetInternalformati64v(GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint64 *params){
	    glad_debug_glGetInternalformati64v(target, internalformat, pname, count, params);
	}
#endif // glGetInternalformati64v

#ifdef glGetInternalformativ
#undef glGetInternalformativ
	static inline void glGetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint *params){
	    glad_debug_glGetInternalformativ(target, internalformat, pname, count, params);
	}
#endif // glGetInternalformativ

#ifdef glGetInvariantBooleanvEXT
#undef glGetInvariantBooleanvEXT
	static inline void glGetInvariantBooleanvEXT(GLuint id, GLenum value, GLboolean *data){
	    glad_debug_glGetInvariantBooleanvEXT(id, value, data);
	}
#endif // glGetInvariantBooleanvEXT

#ifdef glGetInvariantFloatvEXT
#undef glGetInvariantFloatvEXT
	static inline void glGetInvariantFloatvEXT(GLuint id, GLenum value, GLfloat *data){
	    glad_debug_glGetInvariantFloatvEXT(id, value, data);
	}
#endif // glGetInvariantFloatvEXT

#ifdef glGetInvariantIntegervEXT
#undef glGetInvariantIntegervEXT
	static inline void glGetInvariantIntegervEXT(GLuint id, GLenum value, GLint *data){
	    glad_debug_glGetInvariantIntegervEXT(id, value, data);
	}
#endif // glGetInvariantIntegervEXT

#ifdef glGetLightfv
#undef glGetLightfv
	static inline void glGetLightfv(GLenum light, GLenum pname, GLfloat *params){
	    glad_debug_glGetLightfv(light, pname, params);
	}
#endif // glGetLightfv

#ifdef glGetLightiv
#undef glGetLightiv
	static inline void glGetLightiv(GLenum light, GLenum pname, GLint *params){
	    glad_debug_glGetLightiv(light, pname, params);
	}
#endif // glGetLightiv

#ifdef glGetLightxOES
#undef glGetLightxOES
	static inline void glGetLightxOES(GLenum light, GLenum pname, GLfixed *params){
	    glad_debug_glGetLightxOES(light, pname, params);
	}
#endif // glGetLightxOES

#ifdef glGetLightxv
#undef glGetLightxv
	static inline void glGetLightxv(GLenum light, GLenum pname, GLfixed *params){
	    glad_debug_glGetLightxv(light, pname, params);
	}
#endif // glGetLightxv

#ifdef glGetLightxvOES
#undef glGetLightxvOES
	static inline void glGetLightxvOES(GLenum light, GLenum pname, GLfixed *params){
	    glad_debug_glGetLightxvOES(light, pname, params);
	}
#endif // glGetLightxvOES

#ifdef glGetListParameterfvSGIX
#undef glGetListParameterfvSGIX
	static inline void glGetListParameterfvSGIX(GLuint list, GLenum pname, GLfloat *params){
	    glad_debug_glGetListParameterfvSGIX(list, pname, params);
	}
#endif // glGetListParameterfvSGIX

#ifdef glGetListParameterivSGIX
#undef glGetListParameterivSGIX
	static inline void glGetListParameterivSGIX(GLuint list, GLenum pname, GLint *params){
	    glad_debug_glGetListParameterivSGIX(list, pname, params);
	}
#endif // glGetListParameterivSGIX

#ifdef glGetLocalConstantBooleanvEXT
#undef glGetLocalConstantBooleanvEXT
	static inline void glGetLocalConstantBooleanvEXT(GLuint id, GLenum value, GLboolean *data){
	    glad_debug_glGetLocalConstantBooleanvEXT(id, value, data);
	}
#endif // glGetLocalConstantBooleanvEXT

#ifdef glGetLocalConstantFloatvEXT
#undef glGetLocalConstantFloatvEXT
	static inline void glGetLocalConstantFloatvEXT(GLuint id, GLenum value, GLfloat *data){
	    glad_debug_glGetLocalConstantFloatvEXT(id, value, data);
	}
#endif // glGetLocalConstantFloatvEXT

#ifdef glGetLocalConstantIntegervEXT
#undef glGetLocalConstantIntegervEXT
	static inline void glGetLocalConstantIntegervEXT(GLuint id, GLenum value, GLint *data){
	    glad_debug_glGetLocalConstantIntegervEXT(id, value, data);
	}
#endif // glGetLocalConstantIntegervEXT

#ifdef glGetMapAttribParameterfvNV
#undef glGetMapAttribParameterfvNV
	static inline void glGetMapAttribParameterfvNV(GLenum target, GLuint index, GLenum pname, GLfloat *params){
	    glad_debug_glGetMapAttribParameterfvNV(target, index, pname, params);
	}
#endif // glGetMapAttribParameterfvNV

#ifdef glGetMapAttribParameterivNV
#undef glGetMapAttribParameterivNV
	static inline void glGetMapAttribParameterivNV(GLenum target, GLuint index, GLenum pname, GLint *params){
	    glad_debug_glGetMapAttribParameterivNV(target, index, pname, params);
	}
#endif // glGetMapAttribParameterivNV

#ifdef glGetMapParameterfvNV
#undef glGetMapParameterfvNV
	static inline void glGetMapParameterfvNV(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetMapParameterfvNV(target, pname, params);
	}
#endif // glGetMapParameterfvNV

#ifdef glGetMapParameterivNV
#undef glGetMapParameterivNV
	static inline void glGetMapParameterivNV(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetMapParameterivNV(target, pname, params);
	}
#endif // glGetMapParameterivNV

#ifdef glGetMapdv
#undef glGetMapdv
	static inline void glGetMapdv(GLenum target, GLenum query, GLdouble *v){
	    glad_debug_glGetMapdv(target, query, v);
	}
#endif // glGetMapdv

#ifdef glGetMapfv
#undef glGetMapfv
	static inline void glGetMapfv(GLenum target, GLenum query, GLfloat *v){
	    glad_debug_glGetMapfv(target, query, v);
	}
#endif // glGetMapfv

#ifdef glGetMapiv
#undef glGetMapiv
	static inline void glGetMapiv(GLenum target, GLenum query, GLint *v){
	    glad_debug_glGetMapiv(target, query, v);
	}
#endif // glGetMapiv

#ifdef glGetMapxvOES
#undef glGetMapxvOES
	static inline void glGetMapxvOES(GLenum target, GLenum query, GLfixed *v){
	    glad_debug_glGetMapxvOES(target, query, v);
	}
#endif // glGetMapxvOES

#ifdef glGetMaterialfv
#undef glGetMaterialfv
	static inline void glGetMaterialfv(GLenum face, GLenum pname, GLfloat *params){
	    glad_debug_glGetMaterialfv(face, pname, params);
	}
#endif // glGetMaterialfv

#ifdef glGetMaterialiv
#undef glGetMaterialiv
	static inline void glGetMaterialiv(GLenum face, GLenum pname, GLint *params){
	    glad_debug_glGetMaterialiv(face, pname, params);
	}
#endif // glGetMaterialiv

#ifdef glGetMaterialxOES
#undef glGetMaterialxOES
	static inline void glGetMaterialxOES(GLenum face, GLenum pname, GLfixed param){
	    glad_debug_glGetMaterialxOES(face, pname, param);
	}
#endif // glGetMaterialxOES

#ifdef glGetMaterialxv
#undef glGetMaterialxv
	static inline void glGetMaterialxv(GLenum face, GLenum pname, GLfixed *params){
	    glad_debug_glGetMaterialxv(face, pname, params);
	}
#endif // glGetMaterialxv

#ifdef glGetMaterialxvOES
#undef glGetMaterialxvOES
	static inline void glGetMaterialxvOES(GLenum face, GLenum pname, GLfixed *params){
	    glad_debug_glGetMaterialxvOES(face, pname, params);
	}
#endif // glGetMaterialxvOES

#ifdef glGetMemoryObjectDetachedResourcesuivNV
#undef glGetMemoryObjectDetachedResourcesuivNV
	static inline void glGetMemoryObjectDetachedResourcesuivNV(GLuint memory, GLenum pname, GLint first, GLsizei count, GLuint *params){
	    glad_debug_glGetMemoryObjectDetachedResourcesuivNV(memory, pname, first, count, params);
	}
#endif // glGetMemoryObjectDetachedResourcesuivNV

#ifdef glGetMemoryObjectParameterivEXT
#undef glGetMemoryObjectParameterivEXT
	static inline void glGetMemoryObjectParameterivEXT(GLuint memoryObject, GLenum pname, GLint *params){
	    glad_debug_glGetMemoryObjectParameterivEXT(memoryObject, pname, params);
	}
#endif // glGetMemoryObjectParameterivEXT

#ifdef glGetMinmaxParameterfv
#undef glGetMinmaxParameterfv
	static inline void glGetMinmaxParameterfv(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetMinmaxParameterfv(target, pname, params);
	}
#endif // glGetMinmaxParameterfv

#ifdef glGetMinmaxParameterfvEXT
#undef glGetMinmaxParameterfvEXT
	static inline void glGetMinmaxParameterfvEXT(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetMinmaxParameterfvEXT(target, pname, params);
	}
#endif // glGetMinmaxParameterfvEXT

#ifdef glGetMinmaxParameteriv
#undef glGetMinmaxParameteriv
	static inline void glGetMinmaxParameteriv(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetMinmaxParameteriv(target, pname, params);
	}
#endif // glGetMinmaxParameteriv

#ifdef glGetMinmaxParameterivEXT
#undef glGetMinmaxParameterivEXT
	static inline void glGetMinmaxParameterivEXT(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetMinmaxParameterivEXT(target, pname, params);
	}
#endif // glGetMinmaxParameterivEXT

#ifdef glGetMultiTexEnvfvEXT
#undef glGetMultiTexEnvfvEXT
	static inline void glGetMultiTexEnvfvEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetMultiTexEnvfvEXT(texunit, target, pname, params);
	}
#endif // glGetMultiTexEnvfvEXT

#ifdef glGetMultiTexEnvivEXT
#undef glGetMultiTexEnvivEXT
	static inline void glGetMultiTexEnvivEXT(GLenum texunit, GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetMultiTexEnvivEXT(texunit, target, pname, params);
	}
#endif // glGetMultiTexEnvivEXT

#ifdef glGetMultiTexGendvEXT
#undef glGetMultiTexGendvEXT
	static inline void glGetMultiTexGendvEXT(GLenum texunit, GLenum coord, GLenum pname, GLdouble *params){
	    glad_debug_glGetMultiTexGendvEXT(texunit, coord, pname, params);
	}
#endif // glGetMultiTexGendvEXT

#ifdef glGetMultiTexGenfvEXT
#undef glGetMultiTexGenfvEXT
	static inline void glGetMultiTexGenfvEXT(GLenum texunit, GLenum coord, GLenum pname, GLfloat *params){
	    glad_debug_glGetMultiTexGenfvEXT(texunit, coord, pname, params);
	}
#endif // glGetMultiTexGenfvEXT

#ifdef glGetMultiTexGenivEXT
#undef glGetMultiTexGenivEXT
	static inline void glGetMultiTexGenivEXT(GLenum texunit, GLenum coord, GLenum pname, GLint *params){
	    glad_debug_glGetMultiTexGenivEXT(texunit, coord, pname, params);
	}
#endif // glGetMultiTexGenivEXT

#ifdef glGetMultiTexLevelParameterfvEXT
#undef glGetMultiTexLevelParameterfvEXT
	static inline void glGetMultiTexLevelParameterfvEXT(GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat *params){
	    glad_debug_glGetMultiTexLevelParameterfvEXT(texunit, target, level, pname, params);
	}
#endif // glGetMultiTexLevelParameterfvEXT

#ifdef glGetMultiTexLevelParameterivEXT
#undef glGetMultiTexLevelParameterivEXT
	static inline void glGetMultiTexLevelParameterivEXT(GLenum texunit, GLenum target, GLint level, GLenum pname, GLint *params){
	    glad_debug_glGetMultiTexLevelParameterivEXT(texunit, target, level, pname, params);
	}
#endif // glGetMultiTexLevelParameterivEXT

#ifdef glGetMultiTexParameterIivEXT
#undef glGetMultiTexParameterIivEXT
	static inline void glGetMultiTexParameterIivEXT(GLenum texunit, GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetMultiTexParameterIivEXT(texunit, target, pname, params);
	}
#endif // glGetMultiTexParameterIivEXT

#ifdef glGetMultiTexParameterIuivEXT
#undef glGetMultiTexParameterIuivEXT
	static inline void glGetMultiTexParameterIuivEXT(GLenum texunit, GLenum target, GLenum pname, GLuint *params){
	    glad_debug_glGetMultiTexParameterIuivEXT(texunit, target, pname, params);
	}
#endif // glGetMultiTexParameterIuivEXT

#ifdef glGetMultiTexParameterfvEXT
#undef glGetMultiTexParameterfvEXT
	static inline void glGetMultiTexParameterfvEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetMultiTexParameterfvEXT(texunit, target, pname, params);
	}
#endif // glGetMultiTexParameterfvEXT

#ifdef glGetMultiTexParameterivEXT
#undef glGetMultiTexParameterivEXT
	static inline void glGetMultiTexParameterivEXT(GLenum texunit, GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetMultiTexParameterivEXT(texunit, target, pname, params);
	}
#endif // glGetMultiTexParameterivEXT

#ifdef glGetMultisamplefv
#undef glGetMultisamplefv
	static inline void glGetMultisamplefv(GLenum pname, GLuint index, GLfloat *val){
	    glad_debug_glGetMultisamplefv(pname, index, val);
	}
#endif // glGetMultisamplefv

#ifdef glGetMultisamplefvNV
#undef glGetMultisamplefvNV
	static inline void glGetMultisamplefvNV(GLenum pname, GLuint index, GLfloat *val){
	    glad_debug_glGetMultisamplefvNV(pname, index, val);
	}
#endif // glGetMultisamplefvNV

#ifdef glGetNamedBufferParameteri64v
#undef glGetNamedBufferParameteri64v
	static inline void glGetNamedBufferParameteri64v(GLuint buffer, GLenum pname, GLint64 *params){
	    glad_debug_glGetNamedBufferParameteri64v(buffer, pname, params);
	}
#endif // glGetNamedBufferParameteri64v

#ifdef glGetNamedBufferParameteriv
#undef glGetNamedBufferParameteriv
	static inline void glGetNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint *params){
	    glad_debug_glGetNamedBufferParameteriv(buffer, pname, params);
	}
#endif // glGetNamedBufferParameteriv

#ifdef glGetNamedBufferParameterivEXT
#undef glGetNamedBufferParameterivEXT
	static inline void glGetNamedBufferParameterivEXT(GLuint buffer, GLenum pname, GLint *params){
	    glad_debug_glGetNamedBufferParameterivEXT(buffer, pname, params);
	}
#endif // glGetNamedBufferParameterivEXT

#ifdef glGetNamedBufferParameterui64vNV
#undef glGetNamedBufferParameterui64vNV
	static inline void glGetNamedBufferParameterui64vNV(GLuint buffer, GLenum pname, GLuint64EXT *params){
	    glad_debug_glGetNamedBufferParameterui64vNV(buffer, pname, params);
	}
#endif // glGetNamedBufferParameterui64vNV

#ifdef glGetNamedFramebufferParameterfvAMD
#undef glGetNamedFramebufferParameterfvAMD
	static inline void glGetNamedFramebufferParameterfvAMD(GLuint framebuffer, GLenum pname, GLuint numsamples, GLuint pixelindex, GLsizei size, GLfloat *values){
	    glad_debug_glGetNamedFramebufferParameterfvAMD(framebuffer, pname, numsamples, pixelindex, size, values);
	}
#endif // glGetNamedFramebufferParameterfvAMD

#ifdef glGetNamedFramebufferAttachmentParameteriv
#undef glGetNamedFramebufferAttachmentParameteriv
	static inline void glGetNamedFramebufferAttachmentParameteriv(GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params){
	    glad_debug_glGetNamedFramebufferAttachmentParameteriv(framebuffer, attachment, pname, params);
	}
#endif // glGetNamedFramebufferAttachmentParameteriv

#ifdef glGetNamedFramebufferAttachmentParameterivEXT
#undef glGetNamedFramebufferAttachmentParameterivEXT
	static inline void glGetNamedFramebufferAttachmentParameterivEXT(GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params){
	    glad_debug_glGetNamedFramebufferAttachmentParameterivEXT(framebuffer, attachment, pname, params);
	}
#endif // glGetNamedFramebufferAttachmentParameterivEXT

#ifdef glGetNamedFramebufferParameteriv
#undef glGetNamedFramebufferParameteriv
	static inline void glGetNamedFramebufferParameteriv(GLuint framebuffer, GLenum pname, GLint *param){
	    glad_debug_glGetNamedFramebufferParameteriv(framebuffer, pname, param);
	}
#endif // glGetNamedFramebufferParameteriv

#ifdef glGetNamedFramebufferParameterivEXT
#undef glGetNamedFramebufferParameterivEXT
	static inline void glGetNamedFramebufferParameterivEXT(GLuint framebuffer, GLenum pname, GLint *params){
	    glad_debug_glGetNamedFramebufferParameterivEXT(framebuffer, pname, params);
	}
#endif // glGetNamedFramebufferParameterivEXT

#ifdef glGetNamedProgramLocalParameterIivEXT
#undef glGetNamedProgramLocalParameterIivEXT
	static inline void glGetNamedProgramLocalParameterIivEXT(GLuint program, GLenum target, GLuint index, GLint *params){
	    glad_debug_glGetNamedProgramLocalParameterIivEXT(program, target, index, params);
	}
#endif // glGetNamedProgramLocalParameterIivEXT

#ifdef glGetNamedProgramLocalParameterIuivEXT
#undef glGetNamedProgramLocalParameterIuivEXT
	static inline void glGetNamedProgramLocalParameterIuivEXT(GLuint program, GLenum target, GLuint index, GLuint *params){
	    glad_debug_glGetNamedProgramLocalParameterIuivEXT(program, target, index, params);
	}
#endif // glGetNamedProgramLocalParameterIuivEXT

#ifdef glGetNamedProgramLocalParameterdvEXT
#undef glGetNamedProgramLocalParameterdvEXT
	static inline void glGetNamedProgramLocalParameterdvEXT(GLuint program, GLenum target, GLuint index, GLdouble *params){
	    glad_debug_glGetNamedProgramLocalParameterdvEXT(program, target, index, params);
	}
#endif // glGetNamedProgramLocalParameterdvEXT

#ifdef glGetNamedProgramLocalParameterfvEXT
#undef glGetNamedProgramLocalParameterfvEXT
	static inline void glGetNamedProgramLocalParameterfvEXT(GLuint program, GLenum target, GLuint index, GLfloat *params){
	    glad_debug_glGetNamedProgramLocalParameterfvEXT(program, target, index, params);
	}
#endif // glGetNamedProgramLocalParameterfvEXT

#ifdef glGetNamedProgramivEXT
#undef glGetNamedProgramivEXT
	static inline void glGetNamedProgramivEXT(GLuint program, GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetNamedProgramivEXT(program, target, pname, params);
	}
#endif // glGetNamedProgramivEXT

#ifdef glGetNamedRenderbufferParameteriv
#undef glGetNamedRenderbufferParameteriv
	static inline void glGetNamedRenderbufferParameteriv(GLuint renderbuffer, GLenum pname, GLint *params){
	    glad_debug_glGetNamedRenderbufferParameteriv(renderbuffer, pname, params);
	}
#endif // glGetNamedRenderbufferParameteriv

#ifdef glGetNamedRenderbufferParameterivEXT
#undef glGetNamedRenderbufferParameterivEXT
	static inline void glGetNamedRenderbufferParameterivEXT(GLuint renderbuffer, GLenum pname, GLint *params){
	    glad_debug_glGetNamedRenderbufferParameterivEXT(renderbuffer, pname, params);
	}
#endif // glGetNamedRenderbufferParameterivEXT

#ifdef glGetNamedStringARB
#undef glGetNamedStringARB
	static inline void glGetNamedStringARB(GLint namelen, const GLchar *name, GLsizei bufSize, GLint *stringlen, GLchar *string){
	    glad_debug_glGetNamedStringARB(namelen, name, bufSize, stringlen, string);
	}
#endif // glGetNamedStringARB

#ifdef glGetNamedStringivARB
#undef glGetNamedStringivARB
	static inline void glGetNamedStringivARB(GLint namelen, const GLchar *name, GLenum pname, GLint *params){
	    glad_debug_glGetNamedStringivARB(namelen, name, pname, params);
	}
#endif // glGetNamedStringivARB

#ifdef glGetNextPerfQueryIdINTEL
#undef glGetNextPerfQueryIdINTEL
	static inline void glGetNextPerfQueryIdINTEL(GLuint queryId, GLuint *nextQueryId){
	    glad_debug_glGetNextPerfQueryIdINTEL(queryId, nextQueryId);
	}
#endif // glGetNextPerfQueryIdINTEL

#ifdef glGetObjectBufferfvATI
#undef glGetObjectBufferfvATI
	static inline void glGetObjectBufferfvATI(GLuint buffer, GLenum pname, GLfloat *params){
	    glad_debug_glGetObjectBufferfvATI(buffer, pname, params);
	}
#endif // glGetObjectBufferfvATI

#ifdef glGetObjectBufferivATI
#undef glGetObjectBufferivATI
	static inline void glGetObjectBufferivATI(GLuint buffer, GLenum pname, GLint *params){
	    glad_debug_glGetObjectBufferivATI(buffer, pname, params);
	}
#endif // glGetObjectBufferivATI

#ifdef glGetObjectLabel
#undef glGetObjectLabel
	static inline void glGetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label){
	    glad_debug_glGetObjectLabel(identifier, name, bufSize, length, label);
	}
#endif // glGetObjectLabel

#ifdef glGetObjectLabelEXT
#undef glGetObjectLabelEXT
	static inline void glGetObjectLabelEXT(GLenum type, GLuint object, GLsizei bufSize, GLsizei *length, GLchar *label){
	    glad_debug_glGetObjectLabelEXT(type, object, bufSize, length, label);
	}
#endif // glGetObjectLabelEXT

#ifdef glGetObjectLabelKHR
#undef glGetObjectLabelKHR
	static inline void glGetObjectLabelKHR(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label){
	    glad_debug_glGetObjectLabelKHR(identifier, name, bufSize, length, label);
	}
#endif // glGetObjectLabelKHR

#ifdef glGetObjectParameterfvARB
#undef glGetObjectParameterfvARB
	static inline void glGetObjectParameterfvARB(GLhandleARB obj, GLenum pname, GLfloat *params){
	    glad_debug_glGetObjectParameterfvARB(obj, pname, params);
	}
#endif // glGetObjectParameterfvARB

#ifdef glGetObjectParameterivAPPLE
#undef glGetObjectParameterivAPPLE
	static inline void glGetObjectParameterivAPPLE(GLenum objectType, GLuint name, GLenum pname, GLint *params){
	    glad_debug_glGetObjectParameterivAPPLE(objectType, name, pname, params);
	}
#endif // glGetObjectParameterivAPPLE

#ifdef glGetObjectParameterivARB
#undef glGetObjectParameterivARB
	static inline void glGetObjectParameterivARB(GLhandleARB obj, GLenum pname, GLint *params){
	    glad_debug_glGetObjectParameterivARB(obj, pname, params);
	}
#endif // glGetObjectParameterivARB

#ifdef glGetOcclusionQueryivNV
#undef glGetOcclusionQueryivNV
	static inline void glGetOcclusionQueryivNV(GLuint id, GLenum pname, GLint *params){
	    glad_debug_glGetOcclusionQueryivNV(id, pname, params);
	}
#endif // glGetOcclusionQueryivNV

#ifdef glGetOcclusionQueryuivNV
#undef glGetOcclusionQueryuivNV
	static inline void glGetOcclusionQueryuivNV(GLuint id, GLenum pname, GLuint *params){
	    glad_debug_glGetOcclusionQueryuivNV(id, pname, params);
	}
#endif // glGetOcclusionQueryuivNV

#ifdef glGetPathColorGenfvNV
#undef glGetPathColorGenfvNV
	static inline void glGetPathColorGenfvNV(GLenum color, GLenum pname, GLfloat *value){
	    glad_debug_glGetPathColorGenfvNV(color, pname, value);
	}
#endif // glGetPathColorGenfvNV

#ifdef glGetPathColorGenivNV
#undef glGetPathColorGenivNV
	static inline void glGetPathColorGenivNV(GLenum color, GLenum pname, GLint *value){
	    glad_debug_glGetPathColorGenivNV(color, pname, value);
	}
#endif // glGetPathColorGenivNV

#ifdef glGetPathCommandsNV
#undef glGetPathCommandsNV
	static inline void glGetPathCommandsNV(GLuint path, GLubyte *commands){
	    glad_debug_glGetPathCommandsNV(path, commands);
	}
#endif // glGetPathCommandsNV

#ifdef glGetPathCoordsNV
#undef glGetPathCoordsNV
	static inline void glGetPathCoordsNV(GLuint path, GLfloat *coords){
	    glad_debug_glGetPathCoordsNV(path, coords);
	}
#endif // glGetPathCoordsNV

#ifdef glGetPathDashArrayNV
#undef glGetPathDashArrayNV
	static inline void glGetPathDashArrayNV(GLuint path, GLfloat *dashArray){
	    glad_debug_glGetPathDashArrayNV(path, dashArray);
	}
#endif // glGetPathDashArrayNV

#ifdef glGetPathMetricRangeNV
#undef glGetPathMetricRangeNV
	static inline void glGetPathMetricRangeNV(GLbitfield metricQueryMask, GLuint firstPathName, GLsizei numPaths, GLsizei stride, GLfloat *metrics){
	    glad_debug_glGetPathMetricRangeNV(metricQueryMask, firstPathName, numPaths, stride, metrics);
	}
#endif // glGetPathMetricRangeNV

#ifdef glGetPathParameterfvNV
#undef glGetPathParameterfvNV
	static inline void glGetPathParameterfvNV(GLuint path, GLenum pname, GLfloat *value){
	    glad_debug_glGetPathParameterfvNV(path, pname, value);
	}
#endif // glGetPathParameterfvNV

#ifdef glGetPathParameterivNV
#undef glGetPathParameterivNV
	static inline void glGetPathParameterivNV(GLuint path, GLenum pname, GLint *value){
	    glad_debug_glGetPathParameterivNV(path, pname, value);
	}
#endif // glGetPathParameterivNV

#ifdef glGetPathTexGenfvNV
#undef glGetPathTexGenfvNV
	static inline void glGetPathTexGenfvNV(GLenum texCoordSet, GLenum pname, GLfloat *value){
	    glad_debug_glGetPathTexGenfvNV(texCoordSet, pname, value);
	}
#endif // glGetPathTexGenfvNV

#ifdef glGetPathTexGenivNV
#undef glGetPathTexGenivNV
	static inline void glGetPathTexGenivNV(GLenum texCoordSet, GLenum pname, GLint *value){
	    glad_debug_glGetPathTexGenivNV(texCoordSet, pname, value);
	}
#endif // glGetPathTexGenivNV

#ifdef glGetPerfCounterInfoINTEL
#undef glGetPerfCounterInfoINTEL
	static inline void glGetPerfCounterInfoINTEL(GLuint queryId, GLuint counterId, GLuint counterNameLength, GLchar *counterName, GLuint counterDescLength, GLchar *counterDesc, GLuint *counterOffset, GLuint *counterDataSize, GLuint *counterTypeEnum, GLuint *counterDataTypeEnum, GLuint64 *rawCounterMaxValue){
	    glad_debug_glGetPerfCounterInfoINTEL(queryId, counterId, counterNameLength, counterName, counterDescLength, counterDesc, counterOffset, counterDataSize, counterTypeEnum, counterDataTypeEnum, rawCounterMaxValue);
	}
#endif // glGetPerfCounterInfoINTEL

#ifdef glGetPerfMonitorCounterDataAMD
#undef glGetPerfMonitorCounterDataAMD
	static inline void glGetPerfMonitorCounterDataAMD(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint *data, GLint *bytesWritten){
	    glad_debug_glGetPerfMonitorCounterDataAMD(monitor, pname, dataSize, data, bytesWritten);
	}
#endif // glGetPerfMonitorCounterDataAMD

#ifdef glGetPerfMonitorCounterStringAMD
#undef glGetPerfMonitorCounterStringAMD
	static inline void glGetPerfMonitorCounterStringAMD(GLuint group, GLuint counter, GLsizei bufSize, GLsizei *length, GLchar *counterString){
	    glad_debug_glGetPerfMonitorCounterStringAMD(group, counter, bufSize, length, counterString);
	}
#endif // glGetPerfMonitorCounterStringAMD

#ifdef glGetPerfMonitorCountersAMD
#undef glGetPerfMonitorCountersAMD
	static inline void glGetPerfMonitorCountersAMD(GLuint group, GLint *numCounters, GLint *maxActiveCounters, GLsizei counterSize, GLuint *counters){
	    glad_debug_glGetPerfMonitorCountersAMD(group, numCounters, maxActiveCounters, counterSize, counters);
	}
#endif // glGetPerfMonitorCountersAMD

#ifdef glGetPerfMonitorGroupStringAMD
#undef glGetPerfMonitorGroupStringAMD
	static inline void glGetPerfMonitorGroupStringAMD(GLuint group, GLsizei bufSize, GLsizei *length, GLchar *groupString){
	    glad_debug_glGetPerfMonitorGroupStringAMD(group, bufSize, length, groupString);
	}
#endif // glGetPerfMonitorGroupStringAMD

#ifdef glGetPerfMonitorGroupsAMD
#undef glGetPerfMonitorGroupsAMD
	static inline void glGetPerfMonitorGroupsAMD(GLint *numGroups, GLsizei groupsSize, GLuint *groups){
	    glad_debug_glGetPerfMonitorGroupsAMD(numGroups, groupsSize, groups);
	}
#endif // glGetPerfMonitorGroupsAMD

#ifdef glGetPerfQueryIdByNameINTEL
#undef glGetPerfQueryIdByNameINTEL
	static inline void glGetPerfQueryIdByNameINTEL(GLchar *queryName, GLuint *queryId){
	    glad_debug_glGetPerfQueryIdByNameINTEL(queryName, queryId);
	}
#endif // glGetPerfQueryIdByNameINTEL

#ifdef glGetPerfQueryInfoINTEL
#undef glGetPerfQueryInfoINTEL
	static inline void glGetPerfQueryInfoINTEL(GLuint queryId, GLuint queryNameLength, GLchar *queryName, GLuint *dataSize, GLuint *noCounters, GLuint *noInstances, GLuint *capsMask){
	    glad_debug_glGetPerfQueryInfoINTEL(queryId, queryNameLength, queryName, dataSize, noCounters, noInstances, capsMask);
	}
#endif // glGetPerfQueryInfoINTEL

#ifdef glGetPixelMapfv
#undef glGetPixelMapfv
	static inline void glGetPixelMapfv(GLenum map, GLfloat *values){
	    glad_debug_glGetPixelMapfv(map, values);
	}
#endif // glGetPixelMapfv

#ifdef glGetPixelMapuiv
#undef glGetPixelMapuiv
	static inline void glGetPixelMapuiv(GLenum map, GLuint *values){
	    glad_debug_glGetPixelMapuiv(map, values);
	}
#endif // glGetPixelMapuiv

#ifdef glGetPixelMapusv
#undef glGetPixelMapusv
	static inline void glGetPixelMapusv(GLenum map, GLushort *values){
	    glad_debug_glGetPixelMapusv(map, values);
	}
#endif // glGetPixelMapusv

#ifdef glGetPixelMapxv
#undef glGetPixelMapxv
	static inline void glGetPixelMapxv(GLenum map, GLint size, GLfixed *values){
	    glad_debug_glGetPixelMapxv(map, size, values);
	}
#endif // glGetPixelMapxv

#ifdef glGetPixelTexGenParameterfvSGIS
#undef glGetPixelTexGenParameterfvSGIS
	static inline void glGetPixelTexGenParameterfvSGIS(GLenum pname, GLfloat *params){
	    glad_debug_glGetPixelTexGenParameterfvSGIS(pname, params);
	}
#endif // glGetPixelTexGenParameterfvSGIS

#ifdef glGetPixelTexGenParameterivSGIS
#undef glGetPixelTexGenParameterivSGIS
	static inline void glGetPixelTexGenParameterivSGIS(GLenum pname, GLint *params){
	    glad_debug_glGetPixelTexGenParameterivSGIS(pname, params);
	}
#endif // glGetPixelTexGenParameterivSGIS

#ifdef glGetPixelTransformParameterfvEXT
#undef glGetPixelTransformParameterfvEXT
	static inline void glGetPixelTransformParameterfvEXT(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetPixelTransformParameterfvEXT(target, pname, params);
	}
#endif // glGetPixelTransformParameterfvEXT

#ifdef glGetPixelTransformParameterivEXT
#undef glGetPixelTransformParameterivEXT
	static inline void glGetPixelTransformParameterivEXT(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetPixelTransformParameterivEXT(target, pname, params);
	}
#endif // glGetPixelTransformParameterivEXT

#ifdef glGetPolygonStipple
#undef glGetPolygonStipple
	static inline void glGetPolygonStipple(GLubyte *mask){
	    glad_debug_glGetPolygonStipple(mask);
	}
#endif // glGetPolygonStipple

#ifdef glGetProgramEnvParameterIivNV
#undef glGetProgramEnvParameterIivNV
	static inline void glGetProgramEnvParameterIivNV(GLenum target, GLuint index, GLint *params){
	    glad_debug_glGetProgramEnvParameterIivNV(target, index, params);
	}
#endif // glGetProgramEnvParameterIivNV

#ifdef glGetProgramEnvParameterIuivNV
#undef glGetProgramEnvParameterIuivNV
	static inline void glGetProgramEnvParameterIuivNV(GLenum target, GLuint index, GLuint *params){
	    glad_debug_glGetProgramEnvParameterIuivNV(target, index, params);
	}
#endif // glGetProgramEnvParameterIuivNV

#ifdef glGetProgramEnvParameterdvARB
#undef glGetProgramEnvParameterdvARB
	static inline void glGetProgramEnvParameterdvARB(GLenum target, GLuint index, GLdouble *params){
	    glad_debug_glGetProgramEnvParameterdvARB(target, index, params);
	}
#endif // glGetProgramEnvParameterdvARB

#ifdef glGetProgramEnvParameterfvARB
#undef glGetProgramEnvParameterfvARB
	static inline void glGetProgramEnvParameterfvARB(GLenum target, GLuint index, GLfloat *params){
	    glad_debug_glGetProgramEnvParameterfvARB(target, index, params);
	}
#endif // glGetProgramEnvParameterfvARB

#ifdef glGetProgramInfoLog
#undef glGetProgramInfoLog
	static inline void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog){
	    glad_debug_glGetProgramInfoLog(program, bufSize, length, infoLog);
	}
#endif // glGetProgramInfoLog

#ifdef glGetProgramInterfaceiv
#undef glGetProgramInterfaceiv
	static inline void glGetProgramInterfaceiv(GLuint program, GLenum programInterface, GLenum pname, GLint *params){
	    glad_debug_glGetProgramInterfaceiv(program, programInterface, pname, params);
	}
#endif // glGetProgramInterfaceiv

#ifdef glGetProgramLocalParameterIivNV
#undef glGetProgramLocalParameterIivNV
	static inline void glGetProgramLocalParameterIivNV(GLenum target, GLuint index, GLint *params){
	    glad_debug_glGetProgramLocalParameterIivNV(target, index, params);
	}
#endif // glGetProgramLocalParameterIivNV

#ifdef glGetProgramLocalParameterIuivNV
#undef glGetProgramLocalParameterIuivNV
	static inline void glGetProgramLocalParameterIuivNV(GLenum target, GLuint index, GLuint *params){
	    glad_debug_glGetProgramLocalParameterIuivNV(target, index, params);
	}
#endif // glGetProgramLocalParameterIuivNV

#ifdef glGetProgramLocalParameterdvARB
#undef glGetProgramLocalParameterdvARB
	static inline void glGetProgramLocalParameterdvARB(GLenum target, GLuint index, GLdouble *params){
	    glad_debug_glGetProgramLocalParameterdvARB(target, index, params);
	}
#endif // glGetProgramLocalParameterdvARB

#ifdef glGetProgramLocalParameterfvARB
#undef glGetProgramLocalParameterfvARB
	static inline void glGetProgramLocalParameterfvARB(GLenum target, GLuint index, GLfloat *params){
	    glad_debug_glGetProgramLocalParameterfvARB(target, index, params);
	}
#endif // glGetProgramLocalParameterfvARB

#ifdef glGetProgramNamedParameterdvNV
#undef glGetProgramNamedParameterdvNV
	static inline void glGetProgramNamedParameterdvNV(GLuint id, GLsizei len, const GLubyte *name, GLdouble *params){
	    glad_debug_glGetProgramNamedParameterdvNV(id, len, name, params);
	}
#endif // glGetProgramNamedParameterdvNV

#ifdef glGetProgramNamedParameterfvNV
#undef glGetProgramNamedParameterfvNV
	static inline void glGetProgramNamedParameterfvNV(GLuint id, GLsizei len, const GLubyte *name, GLfloat *params){
	    glad_debug_glGetProgramNamedParameterfvNV(id, len, name, params);
	}
#endif // glGetProgramNamedParameterfvNV

#ifdef glGetProgramParameterdvNV
#undef glGetProgramParameterdvNV
	static inline void glGetProgramParameterdvNV(GLenum target, GLuint index, GLenum pname, GLdouble *params){
	    glad_debug_glGetProgramParameterdvNV(target, index, pname, params);
	}
#endif // glGetProgramParameterdvNV

#ifdef glGetProgramParameterfvNV
#undef glGetProgramParameterfvNV
	static inline void glGetProgramParameterfvNV(GLenum target, GLuint index, GLenum pname, GLfloat *params){
	    glad_debug_glGetProgramParameterfvNV(target, index, pname, params);
	}
#endif // glGetProgramParameterfvNV

#ifdef glGetProgramPipelineInfoLog
#undef glGetProgramPipelineInfoLog
	static inline void glGetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog){
	    glad_debug_glGetProgramPipelineInfoLog(pipeline, bufSize, length, infoLog);
	}
#endif // glGetProgramPipelineInfoLog

#ifdef glGetProgramPipelineInfoLogEXT
#undef glGetProgramPipelineInfoLogEXT
	static inline void glGetProgramPipelineInfoLogEXT(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog){
	    glad_debug_glGetProgramPipelineInfoLogEXT(pipeline, bufSize, length, infoLog);
	}
#endif // glGetProgramPipelineInfoLogEXT

#ifdef glGetProgramPipelineiv
#undef glGetProgramPipelineiv
	static inline void glGetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint *params){
	    glad_debug_glGetProgramPipelineiv(pipeline, pname, params);
	}
#endif // glGetProgramPipelineiv

#ifdef glGetProgramPipelineivEXT
#undef glGetProgramPipelineivEXT
	static inline void glGetProgramPipelineivEXT(GLuint pipeline, GLenum pname, GLint *params){
	    glad_debug_glGetProgramPipelineivEXT(pipeline, pname, params);
	}
#endif // glGetProgramPipelineivEXT

#ifdef glGetProgramResourceName
#undef glGetProgramResourceName
	static inline void glGetProgramResourceName(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name){
	    glad_debug_glGetProgramResourceName(program, programInterface, index, bufSize, length, name);
	}
#endif // glGetProgramResourceName

#ifdef glGetProgramResourcefvNV
#undef glGetProgramResourcefvNV
	static inline void glGetProgramResourcefvNV(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei count, GLsizei *length, GLfloat *params){
	    glad_debug_glGetProgramResourcefvNV(program, programInterface, index, propCount, props, count, length, params);
	}
#endif // glGetProgramResourcefvNV

#ifdef glGetProgramResourceiv
#undef glGetProgramResourceiv
	static inline void glGetProgramResourceiv(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei count, GLsizei *length, GLint *params){
	    glad_debug_glGetProgramResourceiv(program, programInterface, index, propCount, props, count, length, params);
	}
#endif // glGetProgramResourceiv

#ifdef glGetProgramStageiv
#undef glGetProgramStageiv
	static inline void glGetProgramStageiv(GLuint program, GLenum shadertype, GLenum pname, GLint *values){
	    glad_debug_glGetProgramStageiv(program, shadertype, pname, values);
	}
#endif // glGetProgramStageiv

#ifdef glGetProgramStringNV
#undef glGetProgramStringNV
	static inline void glGetProgramStringNV(GLuint id, GLenum pname, GLubyte *program){
	    glad_debug_glGetProgramStringNV(id, pname, program);
	}
#endif // glGetProgramStringNV

#ifdef glGetProgramSubroutineParameteruivNV
#undef glGetProgramSubroutineParameteruivNV
	static inline void glGetProgramSubroutineParameteruivNV(GLenum target, GLuint index, GLuint *param){
	    glad_debug_glGetProgramSubroutineParameteruivNV(target, index, param);
	}
#endif // glGetProgramSubroutineParameteruivNV

#ifdef glGetProgramiv
#undef glGetProgramiv
	static inline void glGetProgramiv(GLuint program, GLenum pname, GLint *params){
	    glad_debug_glGetProgramiv(program, pname, params);
	}
#endif // glGetProgramiv

#ifdef glGetProgramivARB
#undef glGetProgramivARB
	static inline void glGetProgramivARB(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetProgramivARB(target, pname, params);
	}
#endif // glGetProgramivARB

#ifdef glGetProgramivNV
#undef glGetProgramivNV
	static inline void glGetProgramivNV(GLuint id, GLenum pname, GLint *params){
	    glad_debug_glGetProgramivNV(id, pname, params);
	}
#endif // glGetProgramivNV

#ifdef glGetQueryBufferObjecti64v
#undef glGetQueryBufferObjecti64v
	static inline void glGetQueryBufferObjecti64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset){
	    glad_debug_glGetQueryBufferObjecti64v(id, buffer, pname, offset);
	}
#endif // glGetQueryBufferObjecti64v

#ifdef glGetQueryBufferObjectiv
#undef glGetQueryBufferObjectiv
	static inline void glGetQueryBufferObjectiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset){
	    glad_debug_glGetQueryBufferObjectiv(id, buffer, pname, offset);
	}
#endif // glGetQueryBufferObjectiv

#ifdef glGetQueryBufferObjectui64v
#undef glGetQueryBufferObjectui64v
	static inline void glGetQueryBufferObjectui64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset){
	    glad_debug_glGetQueryBufferObjectui64v(id, buffer, pname, offset);
	}
#endif // glGetQueryBufferObjectui64v

#ifdef glGetQueryBufferObjectuiv
#undef glGetQueryBufferObjectuiv
	static inline void glGetQueryBufferObjectuiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset){
	    glad_debug_glGetQueryBufferObjectuiv(id, buffer, pname, offset);
	}
#endif // glGetQueryBufferObjectuiv

#ifdef glGetQueryIndexediv
#undef glGetQueryIndexediv
	static inline void glGetQueryIndexediv(GLenum target, GLuint index, GLenum pname, GLint *params){
	    glad_debug_glGetQueryIndexediv(target, index, pname, params);
	}
#endif // glGetQueryIndexediv

#ifdef glGetQueryObjecti64v
#undef glGetQueryObjecti64v
	static inline void glGetQueryObjecti64v(GLuint id, GLenum pname, GLint64 *params){
	    glad_debug_glGetQueryObjecti64v(id, pname, params);
	}
#endif // glGetQueryObjecti64v

#ifdef glGetQueryObjecti64vEXT
#undef glGetQueryObjecti64vEXT
	static inline void glGetQueryObjecti64vEXT(GLuint id, GLenum pname, GLint64 *params){
	    glad_debug_glGetQueryObjecti64vEXT(id, pname, params);
	}
#endif // glGetQueryObjecti64vEXT

#ifdef glGetQueryObjectiv
#undef glGetQueryObjectiv
	static inline void glGetQueryObjectiv(GLuint id, GLenum pname, GLint *params){
	    glad_debug_glGetQueryObjectiv(id, pname, params);
	}
#endif // glGetQueryObjectiv

#ifdef glGetQueryObjectivARB
#undef glGetQueryObjectivARB
	static inline void glGetQueryObjectivARB(GLuint id, GLenum pname, GLint *params){
	    glad_debug_glGetQueryObjectivARB(id, pname, params);
	}
#endif // glGetQueryObjectivARB

#ifdef glGetQueryObjectivEXT
#undef glGetQueryObjectivEXT
	static inline void glGetQueryObjectivEXT(GLuint id, GLenum pname, GLint *params){
	    glad_debug_glGetQueryObjectivEXT(id, pname, params);
	}
#endif // glGetQueryObjectivEXT

#ifdef glGetQueryObjectui64v
#undef glGetQueryObjectui64v
	static inline void glGetQueryObjectui64v(GLuint id, GLenum pname, GLuint64 *params){
	    glad_debug_glGetQueryObjectui64v(id, pname, params);
	}
#endif // glGetQueryObjectui64v

#ifdef glGetQueryObjectui64vEXT
#undef glGetQueryObjectui64vEXT
	static inline void glGetQueryObjectui64vEXT(GLuint id, GLenum pname, GLuint64 *params){
	    glad_debug_glGetQueryObjectui64vEXT(id, pname, params);
	}
#endif // glGetQueryObjectui64vEXT

#ifdef glGetQueryObjectuiv
#undef glGetQueryObjectuiv
	static inline void glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params){
	    glad_debug_glGetQueryObjectuiv(id, pname, params);
	}
#endif // glGetQueryObjectuiv

#ifdef glGetQueryObjectuivARB
#undef glGetQueryObjectuivARB
	static inline void glGetQueryObjectuivARB(GLuint id, GLenum pname, GLuint *params){
	    glad_debug_glGetQueryObjectuivARB(id, pname, params);
	}
#endif // glGetQueryObjectuivARB

#ifdef glGetQueryObjectuivEXT
#undef glGetQueryObjectuivEXT
	static inline void glGetQueryObjectuivEXT(GLuint id, GLenum pname, GLuint *params){
	    glad_debug_glGetQueryObjectuivEXT(id, pname, params);
	}
#endif // glGetQueryObjectuivEXT

#ifdef glGetQueryiv
#undef glGetQueryiv
	static inline void glGetQueryiv(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetQueryiv(target, pname, params);
	}
#endif // glGetQueryiv

#ifdef glGetQueryivARB
#undef glGetQueryivARB
	static inline void glGetQueryivARB(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetQueryivARB(target, pname, params);
	}
#endif // glGetQueryivARB

#ifdef glGetQueryivEXT
#undef glGetQueryivEXT
	static inline void glGetQueryivEXT(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetQueryivEXT(target, pname, params);
	}
#endif // glGetQueryivEXT

#ifdef glGetRenderbufferParameteriv
#undef glGetRenderbufferParameteriv
	static inline void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetRenderbufferParameteriv(target, pname, params);
	}
#endif // glGetRenderbufferParameteriv

#ifdef glGetRenderbufferParameterivEXT
#undef glGetRenderbufferParameterivEXT
	static inline void glGetRenderbufferParameterivEXT(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetRenderbufferParameterivEXT(target, pname, params);
	}
#endif // glGetRenderbufferParameterivEXT

#ifdef glGetRenderbufferParameterivOES
#undef glGetRenderbufferParameterivOES
	static inline void glGetRenderbufferParameterivOES(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetRenderbufferParameterivOES(target, pname, params);
	}
#endif // glGetRenderbufferParameterivOES

#ifdef glGetSamplerParameterIiv
#undef glGetSamplerParameterIiv
	static inline void glGetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint *params){
	    glad_debug_glGetSamplerParameterIiv(sampler, pname, params);
	}
#endif // glGetSamplerParameterIiv

#ifdef glGetSamplerParameterIivEXT
#undef glGetSamplerParameterIivEXT
	static inline void glGetSamplerParameterIivEXT(GLuint sampler, GLenum pname, GLint *params){
	    glad_debug_glGetSamplerParameterIivEXT(sampler, pname, params);
	}
#endif // glGetSamplerParameterIivEXT

#ifdef glGetSamplerParameterIivOES
#undef glGetSamplerParameterIivOES
	static inline void glGetSamplerParameterIivOES(GLuint sampler, GLenum pname, GLint *params){
	    glad_debug_glGetSamplerParameterIivOES(sampler, pname, params);
	}
#endif // glGetSamplerParameterIivOES

#ifdef glGetSamplerParameterIuiv
#undef glGetSamplerParameterIuiv
	static inline void glGetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint *params){
	    glad_debug_glGetSamplerParameterIuiv(sampler, pname, params);
	}
#endif // glGetSamplerParameterIuiv

#ifdef glGetSamplerParameterIuivEXT
#undef glGetSamplerParameterIuivEXT
	static inline void glGetSamplerParameterIuivEXT(GLuint sampler, GLenum pname, GLuint *params){
	    glad_debug_glGetSamplerParameterIuivEXT(sampler, pname, params);
	}
#endif // glGetSamplerParameterIuivEXT

#ifdef glGetSamplerParameterIuivOES
#undef glGetSamplerParameterIuivOES
	static inline void glGetSamplerParameterIuivOES(GLuint sampler, GLenum pname, GLuint *params){
	    glad_debug_glGetSamplerParameterIuivOES(sampler, pname, params);
	}
#endif // glGetSamplerParameterIuivOES

#ifdef glGetSamplerParameterfv
#undef glGetSamplerParameterfv
	static inline void glGetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat *params){
	    glad_debug_glGetSamplerParameterfv(sampler, pname, params);
	}
#endif // glGetSamplerParameterfv

#ifdef glGetSamplerParameteriv
#undef glGetSamplerParameteriv
	static inline void glGetSamplerParameteriv(GLuint sampler, GLenum pname, GLint *params){
	    glad_debug_glGetSamplerParameteriv(sampler, pname, params);
	}
#endif // glGetSamplerParameteriv

#ifdef glGetSemaphoreParameterivNV
#undef glGetSemaphoreParameterivNV
	static inline void glGetSemaphoreParameterivNV(GLuint semaphore, GLenum pname, GLint *params){
	    glad_debug_glGetSemaphoreParameterivNV(semaphore, pname, params);
	}
#endif // glGetSemaphoreParameterivNV

#ifdef glGetSemaphoreParameterui64vEXT
#undef glGetSemaphoreParameterui64vEXT
	static inline void glGetSemaphoreParameterui64vEXT(GLuint semaphore, GLenum pname, GLuint64 *params){
	    glad_debug_glGetSemaphoreParameterui64vEXT(semaphore, pname, params);
	}
#endif // glGetSemaphoreParameterui64vEXT

#ifdef glGetShaderInfoLog
#undef glGetShaderInfoLog
	static inline void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog){
	    glad_debug_glGetShaderInfoLog(shader, bufSize, length, infoLog);
	}
#endif // glGetShaderInfoLog

#ifdef glGetShaderPrecisionFormat
#undef glGetShaderPrecisionFormat
	static inline void glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision){
	    glad_debug_glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
	}
#endif // glGetShaderPrecisionFormat

#ifdef glGetShaderSource
#undef glGetShaderSource
	static inline void glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source){
	    glad_debug_glGetShaderSource(shader, bufSize, length, source);
	}
#endif // glGetShaderSource

#ifdef glGetShaderSourceARB
#undef glGetShaderSourceARB
	static inline void glGetShaderSourceARB(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source){
	    glad_debug_glGetShaderSourceARB(obj, maxLength, length, source);
	}
#endif // glGetShaderSourceARB

#ifdef glGetShaderiv
#undef glGetShaderiv
	static inline void glGetShaderiv(GLuint shader, GLenum pname, GLint *params){
	    glad_debug_glGetShaderiv(shader, pname, params);
	}
#endif // glGetShaderiv

#ifdef glGetShadingRateImagePaletteNV
#undef glGetShadingRateImagePaletteNV
	static inline void glGetShadingRateImagePaletteNV(GLuint viewport, GLuint entry, GLenum *rate){
	    glad_debug_glGetShadingRateImagePaletteNV(viewport, entry, rate);
	}
#endif // glGetShadingRateImagePaletteNV

#ifdef glGetShadingRateSampleLocationivNV
#undef glGetShadingRateSampleLocationivNV
	static inline void glGetShadingRateSampleLocationivNV(GLenum rate, GLuint samples, GLuint index, GLint *location){
	    glad_debug_glGetShadingRateSampleLocationivNV(rate, samples, index, location);
	}
#endif // glGetShadingRateSampleLocationivNV

#ifdef glGetSharpenTexFuncSGIS
#undef glGetSharpenTexFuncSGIS
	static inline void glGetSharpenTexFuncSGIS(GLenum target, GLfloat *points){
	    glad_debug_glGetSharpenTexFuncSGIS(target, points);
	}
#endif // glGetSharpenTexFuncSGIS

#ifdef glGetString
#undef glGetString
	static inline const GLubyte* glGetString(GLenum name){
		return     glad_debug_glGetString(name);
	}
#endif // glGetString

#ifdef glGetStringi
#undef glGetStringi
	static inline const GLubyte* glGetStringi(GLenum name, GLuint index){
		return     glad_debug_glGetStringi(name, index);
	}
#endif // glGetStringi

#ifdef glGetSynciv
#undef glGetSynciv
	static inline void glGetSynciv(GLsync sync, GLenum pname, GLsizei count, GLsizei *length, GLint *values){
	    glad_debug_glGetSynciv(sync, pname, count, length, values);
	}
#endif // glGetSynciv

#ifdef glGetSyncivAPPLE
#undef glGetSyncivAPPLE
	static inline void glGetSyncivAPPLE(GLsync sync, GLenum pname, GLsizei count, GLsizei *length, GLint *values){
	    glad_debug_glGetSyncivAPPLE(sync, pname, count, length, values);
	}
#endif // glGetSyncivAPPLE

#ifdef glGetTexBumpParameterfvATI
#undef glGetTexBumpParameterfvATI
	static inline void glGetTexBumpParameterfvATI(GLenum pname, GLfloat *param){
	    glad_debug_glGetTexBumpParameterfvATI(pname, param);
	}
#endif // glGetTexBumpParameterfvATI

#ifdef glGetTexBumpParameterivATI
#undef glGetTexBumpParameterivATI
	static inline void glGetTexBumpParameterivATI(GLenum pname, GLint *param){
	    glad_debug_glGetTexBumpParameterivATI(pname, param);
	}
#endif // glGetTexBumpParameterivATI

#ifdef glGetTexEnvfv
#undef glGetTexEnvfv
	static inline void glGetTexEnvfv(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetTexEnvfv(target, pname, params);
	}
#endif // glGetTexEnvfv

#ifdef glGetTexEnviv
#undef glGetTexEnviv
	static inline void glGetTexEnviv(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetTexEnviv(target, pname, params);
	}
#endif // glGetTexEnviv

#ifdef glGetTexEnvxv
#undef glGetTexEnvxv
	static inline void glGetTexEnvxv(GLenum target, GLenum pname, GLfixed *params){
	    glad_debug_glGetTexEnvxv(target, pname, params);
	}
#endif // glGetTexEnvxv

#ifdef glGetTexEnvxvOES
#undef glGetTexEnvxvOES
	static inline void glGetTexEnvxvOES(GLenum target, GLenum pname, GLfixed *params){
	    glad_debug_glGetTexEnvxvOES(target, pname, params);
	}
#endif // glGetTexEnvxvOES

#ifdef glGetTexFilterFuncSGIS
#undef glGetTexFilterFuncSGIS
	static inline void glGetTexFilterFuncSGIS(GLenum target, GLenum filter, GLfloat *weights){
	    glad_debug_glGetTexFilterFuncSGIS(target, filter, weights);
	}
#endif // glGetTexFilterFuncSGIS

#ifdef glGetTexGendv
#undef glGetTexGendv
	static inline void glGetTexGendv(GLenum coord, GLenum pname, GLdouble *params){
	    glad_debug_glGetTexGendv(coord, pname, params);
	}
#endif // glGetTexGendv

#ifdef glGetTexGenfv
#undef glGetTexGenfv
	static inline void glGetTexGenfv(GLenum coord, GLenum pname, GLfloat *params){
	    glad_debug_glGetTexGenfv(coord, pname, params);
	}
#endif // glGetTexGenfv

#ifdef glGetTexGenfvOES
#undef glGetTexGenfvOES
	static inline void glGetTexGenfvOES(GLenum coord, GLenum pname, GLfloat *params){
	    glad_debug_glGetTexGenfvOES(coord, pname, params);
	}
#endif // glGetTexGenfvOES

#ifdef glGetTexGeniv
#undef glGetTexGeniv
	static inline void glGetTexGeniv(GLenum coord, GLenum pname, GLint *params){
	    glad_debug_glGetTexGeniv(coord, pname, params);
	}
#endif // glGetTexGeniv

#ifdef glGetTexGenivOES
#undef glGetTexGenivOES
	static inline void glGetTexGenivOES(GLenum coord, GLenum pname, GLint *params){
	    glad_debug_glGetTexGenivOES(coord, pname, params);
	}
#endif // glGetTexGenivOES

#ifdef glGetTexGenxvOES
#undef glGetTexGenxvOES
	static inline void glGetTexGenxvOES(GLenum coord, GLenum pname, GLfixed *params){
	    glad_debug_glGetTexGenxvOES(coord, pname, params);
	}
#endif // glGetTexGenxvOES

#ifdef glGetTexLevelParameterfv
#undef glGetTexLevelParameterfv
	static inline void glGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params){
	    glad_debug_glGetTexLevelParameterfv(target, level, pname, params);
	}
#endif // glGetTexLevelParameterfv

#ifdef glGetTexLevelParameteriv
#undef glGetTexLevelParameteriv
	static inline void glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params){
	    glad_debug_glGetTexLevelParameteriv(target, level, pname, params);
	}
#endif // glGetTexLevelParameteriv

#ifdef glGetTexLevelParameterxvOES
#undef glGetTexLevelParameterxvOES
	static inline void glGetTexLevelParameterxvOES(GLenum target, GLint level, GLenum pname, GLfixed *params){
	    glad_debug_glGetTexLevelParameterxvOES(target, level, pname, params);
	}
#endif // glGetTexLevelParameterxvOES

#ifdef glGetTexParameterIiv
#undef glGetTexParameterIiv
	static inline void glGetTexParameterIiv(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetTexParameterIiv(target, pname, params);
	}
#endif // glGetTexParameterIiv

#ifdef glGetTexParameterIivEXT
#undef glGetTexParameterIivEXT
	static inline void glGetTexParameterIivEXT(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetTexParameterIivEXT(target, pname, params);
	}
#endif // glGetTexParameterIivEXT

#ifdef glGetTexParameterIivOES
#undef glGetTexParameterIivOES
	static inline void glGetTexParameterIivOES(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetTexParameterIivOES(target, pname, params);
	}
#endif // glGetTexParameterIivOES

#ifdef glGetTexParameterIuiv
#undef glGetTexParameterIuiv
	static inline void glGetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params){
	    glad_debug_glGetTexParameterIuiv(target, pname, params);
	}
#endif // glGetTexParameterIuiv

#ifdef glGetTexParameterIuivEXT
#undef glGetTexParameterIuivEXT
	static inline void glGetTexParameterIuivEXT(GLenum target, GLenum pname, GLuint *params){
	    glad_debug_glGetTexParameterIuivEXT(target, pname, params);
	}
#endif // glGetTexParameterIuivEXT

#ifdef glGetTexParameterIuivOES
#undef glGetTexParameterIuivOES
	static inline void glGetTexParameterIuivOES(GLenum target, GLenum pname, GLuint *params){
	    glad_debug_glGetTexParameterIuivOES(target, pname, params);
	}
#endif // glGetTexParameterIuivOES

#ifdef glGetTexParameterfv
#undef glGetTexParameterfv
	static inline void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetTexParameterfv(target, pname, params);
	}
#endif // glGetTexParameterfv

#ifdef glGetTexParameteriv
#undef glGetTexParameteriv
	static inline void glGetTexParameteriv(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetTexParameteriv(target, pname, params);
	}
#endif // glGetTexParameteriv

#ifdef glGetTexParameterxv
#undef glGetTexParameterxv
	static inline void glGetTexParameterxv(GLenum target, GLenum pname, GLfixed *params){
	    glad_debug_glGetTexParameterxv(target, pname, params);
	}
#endif // glGetTexParameterxv

#ifdef glGetTexParameterxvOES
#undef glGetTexParameterxvOES
	static inline void glGetTexParameterxvOES(GLenum target, GLenum pname, GLfixed *params){
	    glad_debug_glGetTexParameterxvOES(target, pname, params);
	}
#endif // glGetTexParameterxvOES

#ifdef glGetTextureLevelParameterfv
#undef glGetTextureLevelParameterfv
	static inline void glGetTextureLevelParameterfv(GLuint texture, GLint level, GLenum pname, GLfloat *params){
	    glad_debug_glGetTextureLevelParameterfv(texture, level, pname, params);
	}
#endif // glGetTextureLevelParameterfv

#ifdef glGetTextureLevelParameterfvEXT
#undef glGetTextureLevelParameterfvEXT
	static inline void glGetTextureLevelParameterfvEXT(GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat *params){
	    glad_debug_glGetTextureLevelParameterfvEXT(texture, target, level, pname, params);
	}
#endif // glGetTextureLevelParameterfvEXT

#ifdef glGetTextureLevelParameteriv
#undef glGetTextureLevelParameteriv
	static inline void glGetTextureLevelParameteriv(GLuint texture, GLint level, GLenum pname, GLint *params){
	    glad_debug_glGetTextureLevelParameteriv(texture, level, pname, params);
	}
#endif // glGetTextureLevelParameteriv

#ifdef glGetTextureLevelParameterivEXT
#undef glGetTextureLevelParameterivEXT
	static inline void glGetTextureLevelParameterivEXT(GLuint texture, GLenum target, GLint level, GLenum pname, GLint *params){
	    glad_debug_glGetTextureLevelParameterivEXT(texture, target, level, pname, params);
	}
#endif // glGetTextureLevelParameterivEXT

#ifdef glGetTextureParameterIiv
#undef glGetTextureParameterIiv
	static inline void glGetTextureParameterIiv(GLuint texture, GLenum pname, GLint *params){
	    glad_debug_glGetTextureParameterIiv(texture, pname, params);
	}
#endif // glGetTextureParameterIiv

#ifdef glGetTextureParameterIivEXT
#undef glGetTextureParameterIivEXT
	static inline void glGetTextureParameterIivEXT(GLuint texture, GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetTextureParameterIivEXT(texture, target, pname, params);
	}
#endif // glGetTextureParameterIivEXT

#ifdef glGetTextureParameterIuiv
#undef glGetTextureParameterIuiv
	static inline void glGetTextureParameterIuiv(GLuint texture, GLenum pname, GLuint *params){
	    glad_debug_glGetTextureParameterIuiv(texture, pname, params);
	}
#endif // glGetTextureParameterIuiv

#ifdef glGetTextureParameterIuivEXT
#undef glGetTextureParameterIuivEXT
	static inline void glGetTextureParameterIuivEXT(GLuint texture, GLenum target, GLenum pname, GLuint *params){
	    glad_debug_glGetTextureParameterIuivEXT(texture, target, pname, params);
	}
#endif // glGetTextureParameterIuivEXT

#ifdef glGetTextureParameterfv
#undef glGetTextureParameterfv
	static inline void glGetTextureParameterfv(GLuint texture, GLenum pname, GLfloat *params){
	    glad_debug_glGetTextureParameterfv(texture, pname, params);
	}
#endif // glGetTextureParameterfv

#ifdef glGetTextureParameterfvEXT
#undef glGetTextureParameterfvEXT
	static inline void glGetTextureParameterfvEXT(GLuint texture, GLenum target, GLenum pname, GLfloat *params){
	    glad_debug_glGetTextureParameterfvEXT(texture, target, pname, params);
	}
#endif // glGetTextureParameterfvEXT

#ifdef glGetTextureParameteriv
#undef glGetTextureParameteriv
	static inline void glGetTextureParameteriv(GLuint texture, GLenum pname, GLint *params){
	    glad_debug_glGetTextureParameteriv(texture, pname, params);
	}
#endif // glGetTextureParameteriv

#ifdef glGetTextureParameterivEXT
#undef glGetTextureParameterivEXT
	static inline void glGetTextureParameterivEXT(GLuint texture, GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetTextureParameterivEXT(texture, target, pname, params);
	}
#endif // glGetTextureParameterivEXT

#ifdef glGetTrackMatrixivNV
#undef glGetTrackMatrixivNV
	static inline void glGetTrackMatrixivNV(GLenum target, GLuint address, GLenum pname, GLint *params){
	    glad_debug_glGetTrackMatrixivNV(target, address, pname, params);
	}
#endif // glGetTrackMatrixivNV

#ifdef glGetTransformFeedbackVarying
#undef glGetTransformFeedbackVarying
	static inline void glGetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name){
	    glad_debug_glGetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);
	}
#endif // glGetTransformFeedbackVarying

#ifdef glGetTransformFeedbackVaryingEXT
#undef glGetTransformFeedbackVaryingEXT
	static inline void glGetTransformFeedbackVaryingEXT(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name){
	    glad_debug_glGetTransformFeedbackVaryingEXT(program, index, bufSize, length, size, type, name);
	}
#endif // glGetTransformFeedbackVaryingEXT

#ifdef glGetTransformFeedbackVaryingNV
#undef glGetTransformFeedbackVaryingNV
	static inline void glGetTransformFeedbackVaryingNV(GLuint program, GLuint index, GLint *location){
	    glad_debug_glGetTransformFeedbackVaryingNV(program, index, location);
	}
#endif // glGetTransformFeedbackVaryingNV

#ifdef glGetTransformFeedbacki64_v
#undef glGetTransformFeedbacki64_v
	static inline void glGetTransformFeedbacki64_v(GLuint xfb, GLenum pname, GLuint index, GLint64 *param){
	    glad_debug_glGetTransformFeedbacki64_v(xfb, pname, index, param);
	}
#endif // glGetTransformFeedbacki64_v

#ifdef glGetTransformFeedbacki_v
#undef glGetTransformFeedbacki_v
	static inline void glGetTransformFeedbacki_v(GLuint xfb, GLenum pname, GLuint index, GLint *param){
	    glad_debug_glGetTransformFeedbacki_v(xfb, pname, index, param);
	}
#endif // glGetTransformFeedbacki_v

#ifdef glGetTransformFeedbackiv
#undef glGetTransformFeedbackiv
	static inline void glGetTransformFeedbackiv(GLuint xfb, GLenum pname, GLint *param){
	    glad_debug_glGetTransformFeedbackiv(xfb, pname, param);
	}
#endif // glGetTransformFeedbackiv

#ifdef glGetTranslatedShaderSourceANGLE
#undef glGetTranslatedShaderSourceANGLE
	static inline void glGetTranslatedShaderSourceANGLE(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source){
	    glad_debug_glGetTranslatedShaderSourceANGLE(shader, bufSize, length, source);
	}
#endif // glGetTranslatedShaderSourceANGLE

#ifdef glGetUniformIndices
#undef glGetUniformIndices
	static inline void glGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices){
	    glad_debug_glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
	}
#endif // glGetUniformIndices

#ifdef glGetUniformSubroutineuiv
#undef glGetUniformSubroutineuiv
	static inline void glGetUniformSubroutineuiv(GLenum shadertype, GLint location, GLuint *params){
	    glad_debug_glGetUniformSubroutineuiv(shadertype, location, params);
	}
#endif // glGetUniformSubroutineuiv

#ifdef glGetUniformdv
#undef glGetUniformdv
	static inline void glGetUniformdv(GLuint program, GLint location, GLdouble *params){
	    glad_debug_glGetUniformdv(program, location, params);
	}
#endif // glGetUniformdv

#ifdef glGetUniformfv
#undef glGetUniformfv
	static inline void glGetUniformfv(GLuint program, GLint location, GLfloat *params){
	    glad_debug_glGetUniformfv(program, location, params);
	}
#endif // glGetUniformfv

#ifdef glGetUniformfvARB
#undef glGetUniformfvARB
	static inline void glGetUniformfvARB(GLhandleARB programObj, GLint location, GLfloat *params){
	    glad_debug_glGetUniformfvARB(programObj, location, params);
	}
#endif // glGetUniformfvARB

#ifdef glGetUniformi64vARB
#undef glGetUniformi64vARB
	static inline void glGetUniformi64vARB(GLuint program, GLint location, GLint64 *params){
	    glad_debug_glGetUniformi64vARB(program, location, params);
	}
#endif // glGetUniformi64vARB

#ifdef glGetUniformi64vNV
#undef glGetUniformi64vNV
	static inline void glGetUniformi64vNV(GLuint program, GLint location, GLint64EXT *params){
	    glad_debug_glGetUniformi64vNV(program, location, params);
	}
#endif // glGetUniformi64vNV

#ifdef glGetUniformiv
#undef glGetUniformiv
	static inline void glGetUniformiv(GLuint program, GLint location, GLint *params){
	    glad_debug_glGetUniformiv(program, location, params);
	}
#endif // glGetUniformiv

#ifdef glGetUniformivARB
#undef glGetUniformivARB
	static inline void glGetUniformivARB(GLhandleARB programObj, GLint location, GLint *params){
	    glad_debug_glGetUniformivARB(programObj, location, params);
	}
#endif // glGetUniformivARB

#ifdef glGetUniformui64vARB
#undef glGetUniformui64vARB
	static inline void glGetUniformui64vARB(GLuint program, GLint location, GLuint64 *params){
	    glad_debug_glGetUniformui64vARB(program, location, params);
	}
#endif // glGetUniformui64vARB

#ifdef glGetUniformui64vNV
#undef glGetUniformui64vNV
	static inline void glGetUniformui64vNV(GLuint program, GLint location, GLuint64EXT *params){
	    glad_debug_glGetUniformui64vNV(program, location, params);
	}
#endif // glGetUniformui64vNV

#ifdef glGetUniformuiv
#undef glGetUniformuiv
	static inline void glGetUniformuiv(GLuint program, GLint location, GLuint *params){
	    glad_debug_glGetUniformuiv(program, location, params);
	}
#endif // glGetUniformuiv

#ifdef glGetUniformuivEXT
#undef glGetUniformuivEXT
	static inline void glGetUniformuivEXT(GLuint program, GLint location, GLuint *params){
	    glad_debug_glGetUniformuivEXT(program, location, params);
	}
#endif // glGetUniformuivEXT

#ifdef glGetUnsignedBytevEXT
#undef glGetUnsignedBytevEXT
	static inline void glGetUnsignedBytevEXT(GLenum pname, GLubyte *data){
	    glad_debug_glGetUnsignedBytevEXT(pname, data);
	}
#endif // glGetUnsignedBytevEXT

#ifdef glGetUnsignedBytei_vEXT
#undef glGetUnsignedBytei_vEXT
	static inline void glGetUnsignedBytei_vEXT(GLenum target, GLuint index, GLubyte *data){
	    glad_debug_glGetUnsignedBytei_vEXT(target, index, data);
	}
#endif // glGetUnsignedBytei_vEXT

#ifdef glGetVariantArrayObjectfvATI
#undef glGetVariantArrayObjectfvATI
	static inline void glGetVariantArrayObjectfvATI(GLuint id, GLenum pname, GLfloat *params){
	    glad_debug_glGetVariantArrayObjectfvATI(id, pname, params);
	}
#endif // glGetVariantArrayObjectfvATI

#ifdef glGetVariantArrayObjectivATI
#undef glGetVariantArrayObjectivATI
	static inline void glGetVariantArrayObjectivATI(GLuint id, GLenum pname, GLint *params){
	    glad_debug_glGetVariantArrayObjectivATI(id, pname, params);
	}
#endif // glGetVariantArrayObjectivATI

#ifdef glGetVariantBooleanvEXT
#undef glGetVariantBooleanvEXT
	static inline void glGetVariantBooleanvEXT(GLuint id, GLenum value, GLboolean *data){
	    glad_debug_glGetVariantBooleanvEXT(id, value, data);
	}
#endif // glGetVariantBooleanvEXT

#ifdef glGetVariantFloatvEXT
#undef glGetVariantFloatvEXT
	static inline void glGetVariantFloatvEXT(GLuint id, GLenum value, GLfloat *data){
	    glad_debug_glGetVariantFloatvEXT(id, value, data);
	}
#endif // glGetVariantFloatvEXT

#ifdef glGetVariantIntegervEXT
#undef glGetVariantIntegervEXT
	static inline void glGetVariantIntegervEXT(GLuint id, GLenum value, GLint *data){
	    glad_debug_glGetVariantIntegervEXT(id, value, data);
	}
#endif // glGetVariantIntegervEXT

#ifdef glGetVertexArrayIndexed64iv
#undef glGetVertexArrayIndexed64iv
	static inline void glGetVertexArrayIndexed64iv(GLuint vaobj, GLuint index, GLenum pname, GLint64 *param){
	    glad_debug_glGetVertexArrayIndexed64iv(vaobj, index, pname, param);
	}
#endif // glGetVertexArrayIndexed64iv

#ifdef glGetVertexArrayIndexediv
#undef glGetVertexArrayIndexediv
	static inline void glGetVertexArrayIndexediv(GLuint vaobj, GLuint index, GLenum pname, GLint *param){
	    glad_debug_glGetVertexArrayIndexediv(vaobj, index, pname, param);
	}
#endif // glGetVertexArrayIndexediv

#ifdef glGetVertexArrayIntegeri_vEXT
#undef glGetVertexArrayIntegeri_vEXT
	static inline void glGetVertexArrayIntegeri_vEXT(GLuint vaobj, GLuint index, GLenum pname, GLint *param){
	    glad_debug_glGetVertexArrayIntegeri_vEXT(vaobj, index, pname, param);
	}
#endif // glGetVertexArrayIntegeri_vEXT

#ifdef glGetVertexArrayIntegervEXT
#undef glGetVertexArrayIntegervEXT
	static inline void glGetVertexArrayIntegervEXT(GLuint vaobj, GLenum pname, GLint *param){
	    glad_debug_glGetVertexArrayIntegervEXT(vaobj, pname, param);
	}
#endif // glGetVertexArrayIntegervEXT

#ifdef glGetVertexArrayiv
#undef glGetVertexArrayiv
	static inline void glGetVertexArrayiv(GLuint vaobj, GLenum pname, GLint *param){
	    glad_debug_glGetVertexArrayiv(vaobj, pname, param);
	}
#endif // glGetVertexArrayiv

#ifdef glGetVertexAttribArrayObjectfvATI
#undef glGetVertexAttribArrayObjectfvATI
	static inline void glGetVertexAttribArrayObjectfvATI(GLuint index, GLenum pname, GLfloat *params){
	    glad_debug_glGetVertexAttribArrayObjectfvATI(index, pname, params);
	}
#endif // glGetVertexAttribArrayObjectfvATI

#ifdef glGetVertexAttribArrayObjectivATI
#undef glGetVertexAttribArrayObjectivATI
	static inline void glGetVertexAttribArrayObjectivATI(GLuint index, GLenum pname, GLint *params){
	    glad_debug_glGetVertexAttribArrayObjectivATI(index, pname, params);
	}
#endif // glGetVertexAttribArrayObjectivATI

#ifdef glGetVertexAttribIiv
#undef glGetVertexAttribIiv
	static inline void glGetVertexAttribIiv(GLuint index, GLenum pname, GLint *params){
	    glad_debug_glGetVertexAttribIiv(index, pname, params);
	}
#endif // glGetVertexAttribIiv

#ifdef glGetVertexAttribIivEXT
#undef glGetVertexAttribIivEXT
	static inline void glGetVertexAttribIivEXT(GLuint index, GLenum pname, GLint *params){
	    glad_debug_glGetVertexAttribIivEXT(index, pname, params);
	}
#endif // glGetVertexAttribIivEXT

#ifdef glGetVertexAttribIuiv
#undef glGetVertexAttribIuiv
	static inline void glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint *params){
	    glad_debug_glGetVertexAttribIuiv(index, pname, params);
	}
#endif // glGetVertexAttribIuiv

#ifdef glGetVertexAttribIuivEXT
#undef glGetVertexAttribIuivEXT
	static inline void glGetVertexAttribIuivEXT(GLuint index, GLenum pname, GLuint *params){
	    glad_debug_glGetVertexAttribIuivEXT(index, pname, params);
	}
#endif // glGetVertexAttribIuivEXT

#ifdef glGetVertexAttribLdv
#undef glGetVertexAttribLdv
	static inline void glGetVertexAttribLdv(GLuint index, GLenum pname, GLdouble *params){
	    glad_debug_glGetVertexAttribLdv(index, pname, params);
	}
#endif // glGetVertexAttribLdv

#ifdef glGetVertexAttribLdvEXT
#undef glGetVertexAttribLdvEXT
	static inline void glGetVertexAttribLdvEXT(GLuint index, GLenum pname, GLdouble *params){
	    glad_debug_glGetVertexAttribLdvEXT(index, pname, params);
	}
#endif // glGetVertexAttribLdvEXT

#ifdef glGetVertexAttribLi64vNV
#undef glGetVertexAttribLi64vNV
	static inline void glGetVertexAttribLi64vNV(GLuint index, GLenum pname, GLint64EXT *params){
	    glad_debug_glGetVertexAttribLi64vNV(index, pname, params);
	}
#endif // glGetVertexAttribLi64vNV

#ifdef glGetVertexAttribLui64vARB
#undef glGetVertexAttribLui64vARB
	static inline void glGetVertexAttribLui64vARB(GLuint index, GLenum pname, GLuint64EXT *params){
	    glad_debug_glGetVertexAttribLui64vARB(index, pname, params);
	}
#endif // glGetVertexAttribLui64vARB

#ifdef glGetVertexAttribLui64vNV
#undef glGetVertexAttribLui64vNV
	static inline void glGetVertexAttribLui64vNV(GLuint index, GLenum pname, GLuint64EXT *params){
	    glad_debug_glGetVertexAttribLui64vNV(index, pname, params);
	}
#endif // glGetVertexAttribLui64vNV

#ifdef glGetVertexAttribdv
#undef glGetVertexAttribdv
	static inline void glGetVertexAttribdv(GLuint index, GLenum pname, GLdouble *params){
	    glad_debug_glGetVertexAttribdv(index, pname, params);
	}
#endif // glGetVertexAttribdv

#ifdef glGetVertexAttribdvARB
#undef glGetVertexAttribdvARB
	static inline void glGetVertexAttribdvARB(GLuint index, GLenum pname, GLdouble *params){
	    glad_debug_glGetVertexAttribdvARB(index, pname, params);
	}
#endif // glGetVertexAttribdvARB

#ifdef glGetVertexAttribdvNV
#undef glGetVertexAttribdvNV
	static inline void glGetVertexAttribdvNV(GLuint index, GLenum pname, GLdouble *params){
	    glad_debug_glGetVertexAttribdvNV(index, pname, params);
	}
#endif // glGetVertexAttribdvNV

#ifdef glGetVertexAttribfv
#undef glGetVertexAttribfv
	static inline void glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params){
	    glad_debug_glGetVertexAttribfv(index, pname, params);
	}
#endif // glGetVertexAttribfv

#ifdef glGetVertexAttribfvARB
#undef glGetVertexAttribfvARB
	static inline void glGetVertexAttribfvARB(GLuint index, GLenum pname, GLfloat *params){
	    glad_debug_glGetVertexAttribfvARB(index, pname, params);
	}
#endif // glGetVertexAttribfvARB

#ifdef glGetVertexAttribfvNV
#undef glGetVertexAttribfvNV
	static inline void glGetVertexAttribfvNV(GLuint index, GLenum pname, GLfloat *params){
	    glad_debug_glGetVertexAttribfvNV(index, pname, params);
	}
#endif // glGetVertexAttribfvNV

#ifdef glGetVertexAttribiv
#undef glGetVertexAttribiv
	static inline void glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params){
	    glad_debug_glGetVertexAttribiv(index, pname, params);
	}
#endif // glGetVertexAttribiv

#ifdef glGetVertexAttribivARB
#undef glGetVertexAttribivARB
	static inline void glGetVertexAttribivARB(GLuint index, GLenum pname, GLint *params){
	    glad_debug_glGetVertexAttribivARB(index, pname, params);
	}
#endif // glGetVertexAttribivARB

#ifdef glGetVertexAttribivNV
#undef glGetVertexAttribivNV
	static inline void glGetVertexAttribivNV(GLuint index, GLenum pname, GLint *params){
	    glad_debug_glGetVertexAttribivNV(index, pname, params);
	}
#endif // glGetVertexAttribivNV

#ifdef glGetVideoCaptureStreamdvNV
#undef glGetVideoCaptureStreamdvNV
	static inline void glGetVideoCaptureStreamdvNV(GLuint video_capture_slot, GLuint stream, GLenum pname, GLdouble *params){
	    glad_debug_glGetVideoCaptureStreamdvNV(video_capture_slot, stream, pname, params);
	}
#endif // glGetVideoCaptureStreamdvNV

#ifdef glGetVideoCaptureStreamfvNV
#undef glGetVideoCaptureStreamfvNV
	static inline void glGetVideoCaptureStreamfvNV(GLuint video_capture_slot, GLuint stream, GLenum pname, GLfloat *params){
	    glad_debug_glGetVideoCaptureStreamfvNV(video_capture_slot, stream, pname, params);
	}
#endif // glGetVideoCaptureStreamfvNV

#ifdef glGetVideoCaptureStreamivNV
#undef glGetVideoCaptureStreamivNV
	static inline void glGetVideoCaptureStreamivNV(GLuint video_capture_slot, GLuint stream, GLenum pname, GLint *params){
	    glad_debug_glGetVideoCaptureStreamivNV(video_capture_slot, stream, pname, params);
	}
#endif // glGetVideoCaptureStreamivNV

#ifdef glGetVideoCaptureivNV
#undef glGetVideoCaptureivNV
	static inline void glGetVideoCaptureivNV(GLuint video_capture_slot, GLenum pname, GLint *params){
	    glad_debug_glGetVideoCaptureivNV(video_capture_slot, pname, params);
	}
#endif // glGetVideoCaptureivNV

#ifdef glGetVideoi64vNV
#undef glGetVideoi64vNV
	static inline void glGetVideoi64vNV(GLuint video_slot, GLenum pname, GLint64EXT *params){
	    glad_debug_glGetVideoi64vNV(video_slot, pname, params);
	}
#endif // glGetVideoi64vNV

#ifdef glGetVideoivNV
#undef glGetVideoivNV
	static inline void glGetVideoivNV(GLuint video_slot, GLenum pname, GLint *params){
	    glad_debug_glGetVideoivNV(video_slot, pname, params);
	}
#endif // glGetVideoivNV

#ifdef glGetVideoui64vNV
#undef glGetVideoui64vNV
	static inline void glGetVideoui64vNV(GLuint video_slot, GLenum pname, GLuint64EXT *params){
	    glad_debug_glGetVideoui64vNV(video_slot, pname, params);
	}
#endif // glGetVideoui64vNV

#ifdef glGetVideouivNV
#undef glGetVideouivNV
	static inline void glGetVideouivNV(GLuint video_slot, GLenum pname, GLuint *params){
	    glad_debug_glGetVideouivNV(video_slot, pname, params);
	}
#endif // glGetVideouivNV

#ifdef glGetnMapdv
#undef glGetnMapdv
	static inline void glGetnMapdv(GLenum target, GLenum query, GLsizei bufSize, GLdouble *v){
	    glad_debug_glGetnMapdv(target, query, bufSize, v);
	}
#endif // glGetnMapdv

#ifdef glGetnMapdvARB
#undef glGetnMapdvARB
	static inline void glGetnMapdvARB(GLenum target, GLenum query, GLsizei bufSize, GLdouble *v){
	    glad_debug_glGetnMapdvARB(target, query, bufSize, v);
	}
#endif // glGetnMapdvARB

#ifdef glGetnMapfv
#undef glGetnMapfv
	static inline void glGetnMapfv(GLenum target, GLenum query, GLsizei bufSize, GLfloat *v){
	    glad_debug_glGetnMapfv(target, query, bufSize, v);
	}
#endif // glGetnMapfv

#ifdef glGetnMapfvARB
#undef glGetnMapfvARB
	static inline void glGetnMapfvARB(GLenum target, GLenum query, GLsizei bufSize, GLfloat *v){
	    glad_debug_glGetnMapfvARB(target, query, bufSize, v);
	}
#endif // glGetnMapfvARB

#ifdef glGetnMapiv
#undef glGetnMapiv
	static inline void glGetnMapiv(GLenum target, GLenum query, GLsizei bufSize, GLint *v){
	    glad_debug_glGetnMapiv(target, query, bufSize, v);
	}
#endif // glGetnMapiv

#ifdef glGetnMapivARB
#undef glGetnMapivARB
	static inline void glGetnMapivARB(GLenum target, GLenum query, GLsizei bufSize, GLint *v){
	    glad_debug_glGetnMapivARB(target, query, bufSize, v);
	}
#endif // glGetnMapivARB

#ifdef glGetnPixelMapfv
#undef glGetnPixelMapfv
	static inline void glGetnPixelMapfv(GLenum map, GLsizei bufSize, GLfloat *values){
	    glad_debug_glGetnPixelMapfv(map, bufSize, values);
	}
#endif // glGetnPixelMapfv

#ifdef glGetnPixelMapfvARB
#undef glGetnPixelMapfvARB
	static inline void glGetnPixelMapfvARB(GLenum map, GLsizei bufSize, GLfloat *values){
	    glad_debug_glGetnPixelMapfvARB(map, bufSize, values);
	}
#endif // glGetnPixelMapfvARB

#ifdef glGetnPixelMapuiv
#undef glGetnPixelMapuiv
	static inline void glGetnPixelMapuiv(GLenum map, GLsizei bufSize, GLuint *values){
	    glad_debug_glGetnPixelMapuiv(map, bufSize, values);
	}
#endif // glGetnPixelMapuiv

#ifdef glGetnPixelMapuivARB
#undef glGetnPixelMapuivARB
	static inline void glGetnPixelMapuivARB(GLenum map, GLsizei bufSize, GLuint *values){
	    glad_debug_glGetnPixelMapuivARB(map, bufSize, values);
	}
#endif // glGetnPixelMapuivARB

#ifdef glGetnPixelMapusv
#undef glGetnPixelMapusv
	static inline void glGetnPixelMapusv(GLenum map, GLsizei bufSize, GLushort *values){
	    glad_debug_glGetnPixelMapusv(map, bufSize, values);
	}
#endif // glGetnPixelMapusv

#ifdef glGetnPixelMapusvARB
#undef glGetnPixelMapusvARB
	static inline void glGetnPixelMapusvARB(GLenum map, GLsizei bufSize, GLushort *values){
	    glad_debug_glGetnPixelMapusvARB(map, bufSize, values);
	}
#endif // glGetnPixelMapusvARB

#ifdef glGetnPolygonStipple
#undef glGetnPolygonStipple
	static inline void glGetnPolygonStipple(GLsizei bufSize, GLubyte *pattern){
	    glad_debug_glGetnPolygonStipple(bufSize, pattern);
	}
#endif // glGetnPolygonStipple

#ifdef glGetnPolygonStippleARB
#undef glGetnPolygonStippleARB
	static inline void glGetnPolygonStippleARB(GLsizei bufSize, GLubyte *pattern){
	    glad_debug_glGetnPolygonStippleARB(bufSize, pattern);
	}
#endif // glGetnPolygonStippleARB

#ifdef glGetnUniformdv
#undef glGetnUniformdv
	static inline void glGetnUniformdv(GLuint program, GLint location, GLsizei bufSize, GLdouble *params){
	    glad_debug_glGetnUniformdv(program, location, bufSize, params);
	}
#endif // glGetnUniformdv

#ifdef glGetnUniformdvARB
#undef glGetnUniformdvARB
	static inline void glGetnUniformdvARB(GLuint program, GLint location, GLsizei bufSize, GLdouble *params){
	    glad_debug_glGetnUniformdvARB(program, location, bufSize, params);
	}
#endif // glGetnUniformdvARB

#ifdef glGetnUniformfv
#undef glGetnUniformfv
	static inline void glGetnUniformfv(GLuint program, GLint location, GLsizei bufSize, GLfloat *params){
	    glad_debug_glGetnUniformfv(program, location, bufSize, params);
	}
#endif // glGetnUniformfv

#ifdef glGetnUniformfvARB
#undef glGetnUniformfvARB
	static inline void glGetnUniformfvARB(GLuint program, GLint location, GLsizei bufSize, GLfloat *params){
	    glad_debug_glGetnUniformfvARB(program, location, bufSize, params);
	}
#endif // glGetnUniformfvARB

#ifdef glGetnUniformfvEXT
#undef glGetnUniformfvEXT
	static inline void glGetnUniformfvEXT(GLuint program, GLint location, GLsizei bufSize, GLfloat *params){
	    glad_debug_glGetnUniformfvEXT(program, location, bufSize, params);
	}
#endif // glGetnUniformfvEXT

#ifdef glGetnUniformfvKHR
#undef glGetnUniformfvKHR
	static inline void glGetnUniformfvKHR(GLuint program, GLint location, GLsizei bufSize, GLfloat *params){
	    glad_debug_glGetnUniformfvKHR(program, location, bufSize, params);
	}
#endif // glGetnUniformfvKHR

#ifdef glGetnUniformi64vARB
#undef glGetnUniformi64vARB
	static inline void glGetnUniformi64vARB(GLuint program, GLint location, GLsizei bufSize, GLint64 *params){
	    glad_debug_glGetnUniformi64vARB(program, location, bufSize, params);
	}
#endif // glGetnUniformi64vARB

#ifdef glGetnUniformiv
#undef glGetnUniformiv
	static inline void glGetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params){
	    glad_debug_glGetnUniformiv(program, location, bufSize, params);
	}
#endif // glGetnUniformiv

#ifdef glGetnUniformivARB
#undef glGetnUniformivARB
	static inline void glGetnUniformivARB(GLuint program, GLint location, GLsizei bufSize, GLint *params){
	    glad_debug_glGetnUniformivARB(program, location, bufSize, params);
	}
#endif // glGetnUniformivARB

#ifdef glGetnUniformivEXT
#undef glGetnUniformivEXT
	static inline void glGetnUniformivEXT(GLuint program, GLint location, GLsizei bufSize, GLint *params){
	    glad_debug_glGetnUniformivEXT(program, location, bufSize, params);
	}
#endif // glGetnUniformivEXT

#ifdef glGetnUniformivKHR
#undef glGetnUniformivKHR
	static inline void glGetnUniformivKHR(GLuint program, GLint location, GLsizei bufSize, GLint *params){
	    glad_debug_glGetnUniformivKHR(program, location, bufSize, params);
	}
#endif // glGetnUniformivKHR

#ifdef glGetnUniformui64vARB
#undef glGetnUniformui64vARB
	static inline void glGetnUniformui64vARB(GLuint program, GLint location, GLsizei bufSize, GLuint64 *params){
	    glad_debug_glGetnUniformui64vARB(program, location, bufSize, params);
	}
#endif // glGetnUniformui64vARB

#ifdef glGetnUniformuiv
#undef glGetnUniformuiv
	static inline void glGetnUniformuiv(GLuint program, GLint location, GLsizei bufSize, GLuint *params){
	    glad_debug_glGetnUniformuiv(program, location, bufSize, params);
	}
#endif // glGetnUniformuiv

#ifdef glGetnUniformuivARB
#undef glGetnUniformuivARB
	static inline void glGetnUniformuivARB(GLuint program, GLint location, GLsizei bufSize, GLuint *params){
	    glad_debug_glGetnUniformuivARB(program, location, bufSize, params);
	}
#endif // glGetnUniformuivARB

#ifdef glGetnUniformuivKHR
#undef glGetnUniformuivKHR
	static inline void glGetnUniformuivKHR(GLuint program, GLint location, GLsizei bufSize, GLuint *params){
	    glad_debug_glGetnUniformuivKHR(program, location, bufSize, params);
	}
#endif // glGetnUniformuivKHR

#ifdef glGlobalAlphaFactorbSUN
#undef glGlobalAlphaFactorbSUN
	static inline void glGlobalAlphaFactorbSUN(GLbyte factor){
	    glad_debug_glGlobalAlphaFactorbSUN(factor);
	}
#endif // glGlobalAlphaFactorbSUN

#ifdef glGlobalAlphaFactordSUN
#undef glGlobalAlphaFactordSUN
	static inline void glGlobalAlphaFactordSUN(GLdouble factor){
	    glad_debug_glGlobalAlphaFactordSUN(factor);
	}
#endif // glGlobalAlphaFactordSUN

#ifdef glGlobalAlphaFactorfSUN
#undef glGlobalAlphaFactorfSUN
	static inline void glGlobalAlphaFactorfSUN(GLfloat factor){
	    glad_debug_glGlobalAlphaFactorfSUN(factor);
	}
#endif // glGlobalAlphaFactorfSUN

#ifdef glGlobalAlphaFactoriSUN
#undef glGlobalAlphaFactoriSUN
	static inline void glGlobalAlphaFactoriSUN(GLint factor){
	    glad_debug_glGlobalAlphaFactoriSUN(factor);
	}
#endif // glGlobalAlphaFactoriSUN

#ifdef glGlobalAlphaFactorsSUN
#undef glGlobalAlphaFactorsSUN
	static inline void glGlobalAlphaFactorsSUN(GLshort factor){
	    glad_debug_glGlobalAlphaFactorsSUN(factor);
	}
#endif // glGlobalAlphaFactorsSUN

#ifdef glGlobalAlphaFactorubSUN
#undef glGlobalAlphaFactorubSUN
	static inline void glGlobalAlphaFactorubSUN(GLubyte factor){
	    glad_debug_glGlobalAlphaFactorubSUN(factor);
	}
#endif // glGlobalAlphaFactorubSUN

#ifdef glGlobalAlphaFactoruiSUN
#undef glGlobalAlphaFactoruiSUN
	static inline void glGlobalAlphaFactoruiSUN(GLuint factor){
	    glad_debug_glGlobalAlphaFactoruiSUN(factor);
	}
#endif // glGlobalAlphaFactoruiSUN

#ifdef glGlobalAlphaFactorusSUN
#undef glGlobalAlphaFactorusSUN
	static inline void glGlobalAlphaFactorusSUN(GLushort factor){
	    glad_debug_glGlobalAlphaFactorusSUN(factor);
	}
#endif // glGlobalAlphaFactorusSUN

#ifdef glHint
#undef glHint
	static inline void glHint(GLenum target, GLenum mode){
	    glad_debug_glHint(target, mode);
	}
#endif // glHint

#ifdef glHintPGI
#undef glHintPGI
	static inline void glHintPGI(GLenum target, GLint mode){
	    glad_debug_glHintPGI(target, mode);
	}
#endif // glHintPGI

#ifdef glHistogram
#undef glHistogram
	static inline void glHistogram(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink){
	    glad_debug_glHistogram(target, width, internalformat, sink);
	}
#endif // glHistogram

#ifdef glHistogramEXT
#undef glHistogramEXT
	static inline void glHistogramEXT(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink){
	    glad_debug_glHistogramEXT(target, width, internalformat, sink);
	}
#endif // glHistogramEXT

#ifdef glImageTransformParameterfHP
#undef glImageTransformParameterfHP
	static inline void glImageTransformParameterfHP(GLenum target, GLenum pname, GLfloat param){
	    glad_debug_glImageTransformParameterfHP(target, pname, param);
	}
#endif // glImageTransformParameterfHP

#ifdef glImageTransformParameterfvHP
#undef glImageTransformParameterfvHP
	static inline void glImageTransformParameterfvHP(GLenum target, GLenum pname, const GLfloat *params){
	    glad_debug_glImageTransformParameterfvHP(target, pname, params);
	}
#endif // glImageTransformParameterfvHP

#ifdef glImageTransformParameteriHP
#undef glImageTransformParameteriHP
	static inline void glImageTransformParameteriHP(GLenum target, GLenum pname, GLint param){
	    glad_debug_glImageTransformParameteriHP(target, pname, param);
	}
#endif // glImageTransformParameteriHP

#ifdef glImageTransformParameterivHP
#undef glImageTransformParameterivHP
	static inline void glImageTransformParameterivHP(GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glImageTransformParameterivHP(target, pname, params);
	}
#endif // glImageTransformParameterivHP

#ifdef glImportMemoryFdEXT
#undef glImportMemoryFdEXT
	static inline void glImportMemoryFdEXT(GLuint memory, GLuint64 size, GLenum handleType, GLint fd){
	    glad_debug_glImportMemoryFdEXT(memory, size, handleType, fd);
	}
#endif // glImportMemoryFdEXT

#ifdef glImportSemaphoreFdEXT
#undef glImportSemaphoreFdEXT
	static inline void glImportSemaphoreFdEXT(GLuint semaphore, GLenum handleType, GLint fd){
	    glad_debug_glImportSemaphoreFdEXT(semaphore, handleType, fd);
	}
#endif // glImportSemaphoreFdEXT

#ifdef glIndexFormatNV
#undef glIndexFormatNV
	static inline void glIndexFormatNV(GLenum type, GLsizei stride){
	    glad_debug_glIndexFormatNV(type, stride);
	}
#endif // glIndexFormatNV

#ifdef glIndexFuncEXT
#undef glIndexFuncEXT
	static inline void glIndexFuncEXT(GLenum func, GLclampf ref){
	    glad_debug_glIndexFuncEXT(func, ref);
	}
#endif // glIndexFuncEXT

#ifdef glIndexMask
#undef glIndexMask
	static inline void glIndexMask(GLuint mask){
	    glad_debug_glIndexMask(mask);
	}
#endif // glIndexMask

#ifdef glIndexMaterialEXT
#undef glIndexMaterialEXT
	static inline void glIndexMaterialEXT(GLenum face, GLenum mode){
	    glad_debug_glIndexMaterialEXT(face, mode);
	}
#endif // glIndexMaterialEXT

#ifdef glIndexd
#undef glIndexd
	static inline void glIndexd(GLdouble c){
	    glad_debug_glIndexd(c);
	}
#endif // glIndexd

#ifdef glIndexdv
#undef glIndexdv
	static inline void glIndexdv(const GLdouble *c){
	    glad_debug_glIndexdv(c);
	}
#endif // glIndexdv

#ifdef glIndexf
#undef glIndexf
	static inline void glIndexf(GLfloat c){
	    glad_debug_glIndexf(c);
	}
#endif // glIndexf

#ifdef glIndexfv
#undef glIndexfv
	static inline void glIndexfv(const GLfloat *c){
	    glad_debug_glIndexfv(c);
	}
#endif // glIndexfv

#ifdef glIndexi
#undef glIndexi
	static inline void glIndexi(GLint c){
	    glad_debug_glIndexi(c);
	}
#endif // glIndexi

#ifdef glIndexiv
#undef glIndexiv
	static inline void glIndexiv(const GLint *c){
	    glad_debug_glIndexiv(c);
	}
#endif // glIndexiv

#ifdef glIndexs
#undef glIndexs
	static inline void glIndexs(GLshort c){
	    glad_debug_glIndexs(c);
	}
#endif // glIndexs

#ifdef glIndexsv
#undef glIndexsv
	static inline void glIndexsv(const GLshort *c){
	    glad_debug_glIndexsv(c);
	}
#endif // glIndexsv

#ifdef glIndexub
#undef glIndexub
	static inline void glIndexub(GLubyte c){
	    glad_debug_glIndexub(c);
	}
#endif // glIndexub

#ifdef glIndexubv
#undef glIndexubv
	static inline void glIndexubv(const GLubyte *c){
	    glad_debug_glIndexubv(c);
	}
#endif // glIndexubv

#ifdef glIndexxOES
#undef glIndexxOES
	static inline void glIndexxOES(GLfixed component){
	    glad_debug_glIndexxOES(component);
	}
#endif // glIndexxOES

#ifdef glIndexxvOES
#undef glIndexxvOES
	static inline void glIndexxvOES(const GLfixed *component){
	    glad_debug_glIndexxvOES(component);
	}
#endif // glIndexxvOES

#ifdef glInitNames
#undef glInitNames
	static inline void glInitNames(){
	    glad_debug_glInitNames();
	}
#endif // glInitNames

#ifdef glInsertComponentEXT
#undef glInsertComponentEXT
	static inline void glInsertComponentEXT(GLuint res, GLuint src, GLuint num){
	    glad_debug_glInsertComponentEXT(res, src, num);
	}
#endif // glInsertComponentEXT

#ifdef glInsertEventMarkerEXT
#undef glInsertEventMarkerEXT
	static inline void glInsertEventMarkerEXT(GLsizei length, const GLchar *marker){
	    glad_debug_glInsertEventMarkerEXT(length, marker);
	}
#endif // glInsertEventMarkerEXT

#ifdef glInstrumentsBufferSGIX
#undef glInstrumentsBufferSGIX
	static inline void glInstrumentsBufferSGIX(GLsizei size, GLint *buffer){
	    glad_debug_glInstrumentsBufferSGIX(size, buffer);
	}
#endif // glInstrumentsBufferSGIX

#ifdef glInterpolatePathsNV
#undef glInterpolatePathsNV
	static inline void glInterpolatePathsNV(GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight){
	    glad_debug_glInterpolatePathsNV(resultPath, pathA, pathB, weight);
	}
#endif // glInterpolatePathsNV

#ifdef glInvalidateBufferData
#undef glInvalidateBufferData
	static inline void glInvalidateBufferData(GLuint buffer){
	    glad_debug_glInvalidateBufferData(buffer);
	}
#endif // glInvalidateBufferData

#ifdef glInvalidateBufferSubData
#undef glInvalidateBufferSubData
	static inline void glInvalidateBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr length){
	    glad_debug_glInvalidateBufferSubData(buffer, offset, length);
	}
#endif // glInvalidateBufferSubData

#ifdef glInvalidateFramebuffer
#undef glInvalidateFramebuffer
	static inline void glInvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments){
	    glad_debug_glInvalidateFramebuffer(target, numAttachments, attachments);
	}
#endif // glInvalidateFramebuffer

#ifdef glInvalidateNamedFramebufferData
#undef glInvalidateNamedFramebufferData
	static inline void glInvalidateNamedFramebufferData(GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments){
	    glad_debug_glInvalidateNamedFramebufferData(framebuffer, numAttachments, attachments);
	}
#endif // glInvalidateNamedFramebufferData

#ifdef glInvalidateNamedFramebufferSubData
#undef glInvalidateNamedFramebufferSubData
	static inline void glInvalidateNamedFramebufferSubData(GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glInvalidateNamedFramebufferSubData(framebuffer, numAttachments, attachments, x, y, width, height);
	}
#endif // glInvalidateNamedFramebufferSubData

#ifdef glInvalidateSubFramebuffer
#undef glInvalidateSubFramebuffer
	static inline void glInvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glInvalidateSubFramebuffer(target, numAttachments, attachments, x, y, width, height);
	}
#endif // glInvalidateSubFramebuffer

#ifdef glInvalidateTexImage
#undef glInvalidateTexImage
	static inline void glInvalidateTexImage(GLuint texture, GLint level){
	    glad_debug_glInvalidateTexImage(texture, level);
	}
#endif // glInvalidateTexImage

#ifdef glInvalidateTexSubImage
#undef glInvalidateTexSubImage
	static inline void glInvalidateTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth){
	    glad_debug_glInvalidateTexSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth);
	}
#endif // glInvalidateTexSubImage

#ifdef glLGPUCopyImageSubDataNVX
#undef glLGPUCopyImageSubDataNVX
	static inline void glLGPUCopyImageSubDataNVX(GLuint sourceGpu, GLbitfield destinationGpuMask, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srxY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth){
	    glad_debug_glLGPUCopyImageSubDataNVX(sourceGpu, destinationGpuMask, srcName, srcTarget, srcLevel, srcX, srxY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);
	}
#endif // glLGPUCopyImageSubDataNVX

#ifdef glLGPUInterlockNVX
#undef glLGPUInterlockNVX
	static inline void glLGPUInterlockNVX(){
	    glad_debug_glLGPUInterlockNVX();
	}
#endif // glLGPUInterlockNVX

#ifdef glLabelObjectEXT
#undef glLabelObjectEXT
	static inline void glLabelObjectEXT(GLenum type, GLuint object, GLsizei length, const GLchar *label){
	    glad_debug_glLabelObjectEXT(type, object, length, label);
	}
#endif // glLabelObjectEXT

#ifdef glLightEnviSGIX
#undef glLightEnviSGIX
	static inline void glLightEnviSGIX(GLenum pname, GLint param){
	    glad_debug_glLightEnviSGIX(pname, param);
	}
#endif // glLightEnviSGIX

#ifdef glLightModelf
#undef glLightModelf
	static inline void glLightModelf(GLenum pname, GLfloat param){
	    glad_debug_glLightModelf(pname, param);
	}
#endif // glLightModelf

#ifdef glLightModelfv
#undef glLightModelfv
	static inline void glLightModelfv(GLenum pname, const GLfloat *params){
	    glad_debug_glLightModelfv(pname, params);
	}
#endif // glLightModelfv

#ifdef glLightModeli
#undef glLightModeli
	static inline void glLightModeli(GLenum pname, GLint param){
	    glad_debug_glLightModeli(pname, param);
	}
#endif // glLightModeli

#ifdef glLightModeliv
#undef glLightModeliv
	static inline void glLightModeliv(GLenum pname, const GLint *params){
	    glad_debug_glLightModeliv(pname, params);
	}
#endif // glLightModeliv

#ifdef glLightModelx
#undef glLightModelx
	static inline void glLightModelx(GLenum pname, GLfixed param){
	    glad_debug_glLightModelx(pname, param);
	}
#endif // glLightModelx

#ifdef glLightModelxOES
#undef glLightModelxOES
	static inline void glLightModelxOES(GLenum pname, GLfixed param){
	    glad_debug_glLightModelxOES(pname, param);
	}
#endif // glLightModelxOES

#ifdef glLightModelxv
#undef glLightModelxv
	static inline void glLightModelxv(GLenum pname, const GLfixed *param){
	    glad_debug_glLightModelxv(pname, param);
	}
#endif // glLightModelxv

#ifdef glLightModelxvOES
#undef glLightModelxvOES
	static inline void glLightModelxvOES(GLenum pname, const GLfixed *param){
	    glad_debug_glLightModelxvOES(pname, param);
	}
#endif // glLightModelxvOES

#ifdef glLightf
#undef glLightf
	static inline void glLightf(GLenum light, GLenum pname, GLfloat param){
	    glad_debug_glLightf(light, pname, param);
	}
#endif // glLightf

#ifdef glLightfv
#undef glLightfv
	static inline void glLightfv(GLenum light, GLenum pname, const GLfloat *params){
	    glad_debug_glLightfv(light, pname, params);
	}
#endif // glLightfv

#ifdef glLighti
#undef glLighti
	static inline void glLighti(GLenum light, GLenum pname, GLint param){
	    glad_debug_glLighti(light, pname, param);
	}
#endif // glLighti

#ifdef glLightiv
#undef glLightiv
	static inline void glLightiv(GLenum light, GLenum pname, const GLint *params){
	    glad_debug_glLightiv(light, pname, params);
	}
#endif // glLightiv

#ifdef glLightx
#undef glLightx
	static inline void glLightx(GLenum light, GLenum pname, GLfixed param){
	    glad_debug_glLightx(light, pname, param);
	}
#endif // glLightx

#ifdef glLightxOES
#undef glLightxOES
	static inline void glLightxOES(GLenum light, GLenum pname, GLfixed param){
	    glad_debug_glLightxOES(light, pname, param);
	}
#endif // glLightxOES

#ifdef glLightxv
#undef glLightxv
	static inline void glLightxv(GLenum light, GLenum pname, const GLfixed *params){
	    glad_debug_glLightxv(light, pname, params);
	}
#endif // glLightxv

#ifdef glLightxvOES
#undef glLightxvOES
	static inline void glLightxvOES(GLenum light, GLenum pname, const GLfixed *params){
	    glad_debug_glLightxvOES(light, pname, params);
	}
#endif // glLightxvOES

#ifdef glLineStipple
#undef glLineStipple
	static inline void glLineStipple(GLint factor, GLushort pattern){
	    glad_debug_glLineStipple(factor, pattern);
	}
#endif // glLineStipple

#ifdef glLineWidth
#undef glLineWidth
	static inline void glLineWidth(GLfloat width){
	    glad_debug_glLineWidth(width);
	}
#endif // glLineWidth

#ifdef glLineWidthx
#undef glLineWidthx
	static inline void glLineWidthx(GLfixed width){
	    glad_debug_glLineWidthx(width);
	}
#endif // glLineWidthx

#ifdef glLineWidthxOES
#undef glLineWidthxOES
	static inline void glLineWidthxOES(GLfixed width){
	    glad_debug_glLineWidthxOES(width);
	}
#endif // glLineWidthxOES

#ifdef glLinkProgram
#undef glLinkProgram
	static inline void glLinkProgram(GLuint program){
	    glad_debug_glLinkProgram(program);
	}
#endif // glLinkProgram

#ifdef glLinkProgramARB
#undef glLinkProgramARB
	static inline void glLinkProgramARB(GLhandleARB programObj){
	    glad_debug_glLinkProgramARB(programObj);
	}
#endif // glLinkProgramARB

#ifdef glListBase
#undef glListBase
	static inline void glListBase(GLuint base){
	    glad_debug_glListBase(base);
	}
#endif // glListBase

#ifdef glListParameterfSGIX
#undef glListParameterfSGIX
	static inline void glListParameterfSGIX(GLuint list, GLenum pname, GLfloat param){
	    glad_debug_glListParameterfSGIX(list, pname, param);
	}
#endif // glListParameterfSGIX

#ifdef glListParameterfvSGIX
#undef glListParameterfvSGIX
	static inline void glListParameterfvSGIX(GLuint list, GLenum pname, const GLfloat *params){
	    glad_debug_glListParameterfvSGIX(list, pname, params);
	}
#endif // glListParameterfvSGIX

#ifdef glListParameteriSGIX
#undef glListParameteriSGIX
	static inline void glListParameteriSGIX(GLuint list, GLenum pname, GLint param){
	    glad_debug_glListParameteriSGIX(list, pname, param);
	}
#endif // glListParameteriSGIX

#ifdef glListParameterivSGIX
#undef glListParameterivSGIX
	static inline void glListParameterivSGIX(GLuint list, GLenum pname, const GLint *params){
	    glad_debug_glListParameterivSGIX(list, pname, params);
	}
#endif // glListParameterivSGIX

#ifdef glLoadIdentity
#undef glLoadIdentity
	static inline void glLoadIdentity(){
	    glad_debug_glLoadIdentity();
	}
#endif // glLoadIdentity

#ifdef glLoadIdentityDeformationMapSGIX
#undef glLoadIdentityDeformationMapSGIX
	static inline void glLoadIdentityDeformationMapSGIX(GLbitfield mask){
	    glad_debug_glLoadIdentityDeformationMapSGIX(mask);
	}
#endif // glLoadIdentityDeformationMapSGIX

#ifdef glLoadMatrixd
#undef glLoadMatrixd
	static inline void glLoadMatrixd(const GLdouble *m){
	    glad_debug_glLoadMatrixd(m);
	}
#endif // glLoadMatrixd

#ifdef glLoadMatrixf
#undef glLoadMatrixf
	static inline void glLoadMatrixf(const GLfloat *m){
	    glad_debug_glLoadMatrixf(m);
	}
#endif // glLoadMatrixf

#ifdef glLoadMatrixx
#undef glLoadMatrixx
	static inline void glLoadMatrixx(const GLfixed *m){
	    glad_debug_glLoadMatrixx(m);
	}
#endif // glLoadMatrixx

#ifdef glLoadMatrixxOES
#undef glLoadMatrixxOES
	static inline void glLoadMatrixxOES(const GLfixed *m){
	    glad_debug_glLoadMatrixxOES(m);
	}
#endif // glLoadMatrixxOES

#ifdef glLoadName
#undef glLoadName
	static inline void glLoadName(GLuint name){
	    glad_debug_glLoadName(name);
	}
#endif // glLoadName

#ifdef glLoadPaletteFromModelViewMatrixOES
#undef glLoadPaletteFromModelViewMatrixOES
	static inline void glLoadPaletteFromModelViewMatrixOES(){
	    glad_debug_glLoadPaletteFromModelViewMatrixOES();
	}
#endif // glLoadPaletteFromModelViewMatrixOES

#ifdef glLoadProgramNV
#undef glLoadProgramNV
	static inline void glLoadProgramNV(GLenum target, GLuint id, GLsizei len, const GLubyte *program){
	    glad_debug_glLoadProgramNV(target, id, len, program);
	}
#endif // glLoadProgramNV

#ifdef glLoadTransposeMatrixd
#undef glLoadTransposeMatrixd
	static inline void glLoadTransposeMatrixd(const GLdouble *m){
	    glad_debug_glLoadTransposeMatrixd(m);
	}
#endif // glLoadTransposeMatrixd

#ifdef glLoadTransposeMatrixdARB
#undef glLoadTransposeMatrixdARB
	static inline void glLoadTransposeMatrixdARB(const GLdouble *m){
	    glad_debug_glLoadTransposeMatrixdARB(m);
	}
#endif // glLoadTransposeMatrixdARB

#ifdef glLoadTransposeMatrixf
#undef glLoadTransposeMatrixf
	static inline void glLoadTransposeMatrixf(const GLfloat *m){
	    glad_debug_glLoadTransposeMatrixf(m);
	}
#endif // glLoadTransposeMatrixf

#ifdef glLoadTransposeMatrixfARB
#undef glLoadTransposeMatrixfARB
	static inline void glLoadTransposeMatrixfARB(const GLfloat *m){
	    glad_debug_glLoadTransposeMatrixfARB(m);
	}
#endif // glLoadTransposeMatrixfARB

#ifdef glLoadTransposeMatrixxOES
#undef glLoadTransposeMatrixxOES
	static inline void glLoadTransposeMatrixxOES(const GLfixed *m){
	    glad_debug_glLoadTransposeMatrixxOES(m);
	}
#endif // glLoadTransposeMatrixxOES

#ifdef glLockArraysEXT
#undef glLockArraysEXT
	static inline void glLockArraysEXT(GLint first, GLsizei count){
	    glad_debug_glLockArraysEXT(first, count);
	}
#endif // glLockArraysEXT

#ifdef glLogicOp
#undef glLogicOp
	static inline void glLogicOp(GLenum opcode){
	    glad_debug_glLogicOp(opcode);
	}
#endif // glLogicOp

#ifdef glMakeBufferNonResidentNV
#undef glMakeBufferNonResidentNV
	static inline void glMakeBufferNonResidentNV(GLenum target){
	    glad_debug_glMakeBufferNonResidentNV(target);
	}
#endif // glMakeBufferNonResidentNV

#ifdef glMakeBufferResidentNV
#undef glMakeBufferResidentNV
	static inline void glMakeBufferResidentNV(GLenum target, GLenum access){
	    glad_debug_glMakeBufferResidentNV(target, access);
	}
#endif // glMakeBufferResidentNV

#ifdef glMakeImageHandleNonResidentARB
#undef glMakeImageHandleNonResidentARB
	static inline void glMakeImageHandleNonResidentARB(GLuint64 handle){
	    glad_debug_glMakeImageHandleNonResidentARB(handle);
	}
#endif // glMakeImageHandleNonResidentARB

#ifdef glMakeImageHandleNonResidentNV
#undef glMakeImageHandleNonResidentNV
	static inline void glMakeImageHandleNonResidentNV(GLuint64 handle){
	    glad_debug_glMakeImageHandleNonResidentNV(handle);
	}
#endif // glMakeImageHandleNonResidentNV

#ifdef glMakeImageHandleResidentARB
#undef glMakeImageHandleResidentARB
	static inline void glMakeImageHandleResidentARB(GLuint64 handle, GLenum access){
	    glad_debug_glMakeImageHandleResidentARB(handle, access);
	}
#endif // glMakeImageHandleResidentARB

#ifdef glMakeImageHandleResidentNV
#undef glMakeImageHandleResidentNV
	static inline void glMakeImageHandleResidentNV(GLuint64 handle, GLenum access){
	    glad_debug_glMakeImageHandleResidentNV(handle, access);
	}
#endif // glMakeImageHandleResidentNV

#ifdef glMakeNamedBufferNonResidentNV
#undef glMakeNamedBufferNonResidentNV
	static inline void glMakeNamedBufferNonResidentNV(GLuint buffer){
	    glad_debug_glMakeNamedBufferNonResidentNV(buffer);
	}
#endif // glMakeNamedBufferNonResidentNV

#ifdef glMakeNamedBufferResidentNV
#undef glMakeNamedBufferResidentNV
	static inline void glMakeNamedBufferResidentNV(GLuint buffer, GLenum access){
	    glad_debug_glMakeNamedBufferResidentNV(buffer, access);
	}
#endif // glMakeNamedBufferResidentNV

#ifdef glMakeTextureHandleNonResidentARB
#undef glMakeTextureHandleNonResidentARB
	static inline void glMakeTextureHandleNonResidentARB(GLuint64 handle){
	    glad_debug_glMakeTextureHandleNonResidentARB(handle);
	}
#endif // glMakeTextureHandleNonResidentARB

#ifdef glMakeTextureHandleNonResidentNV
#undef glMakeTextureHandleNonResidentNV
	static inline void glMakeTextureHandleNonResidentNV(GLuint64 handle){
	    glad_debug_glMakeTextureHandleNonResidentNV(handle);
	}
#endif // glMakeTextureHandleNonResidentNV

#ifdef glMakeTextureHandleResidentARB
#undef glMakeTextureHandleResidentARB
	static inline void glMakeTextureHandleResidentARB(GLuint64 handle){
	    glad_debug_glMakeTextureHandleResidentARB(handle);
	}
#endif // glMakeTextureHandleResidentARB

#ifdef glMakeTextureHandleResidentNV
#undef glMakeTextureHandleResidentNV
	static inline void glMakeTextureHandleResidentNV(GLuint64 handle){
	    glad_debug_glMakeTextureHandleResidentNV(handle);
	}
#endif // glMakeTextureHandleResidentNV

#ifdef glMap1d
#undef glMap1d
	static inline void glMap1d(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points){
	    glad_debug_glMap1d(target, u1, u2, stride, order, points);
	}
#endif // glMap1d

#ifdef glMap1f
#undef glMap1f
	static inline void glMap1f(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points){
	    glad_debug_glMap1f(target, u1, u2, stride, order, points);
	}
#endif // glMap1f

#ifdef glMap1xOES
#undef glMap1xOES
	static inline void glMap1xOES(GLenum target, GLfixed u1, GLfixed u2, GLint stride, GLint order, GLfixed points){
	    glad_debug_glMap1xOES(target, u1, u2, stride, order, points);
	}
#endif // glMap1xOES

#ifdef glMap2d
#undef glMap2d
	static inline void glMap2d(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points){
	    glad_debug_glMap2d(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
	}
#endif // glMap2d

#ifdef glMap2f
#undef glMap2f
	static inline void glMap2f(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points){
	    glad_debug_glMap2f(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
	}
#endif // glMap2f

#ifdef glMap2xOES
#undef glMap2xOES
	static inline void glMap2xOES(GLenum target, GLfixed u1, GLfixed u2, GLint ustride, GLint uorder, GLfixed v1, GLfixed v2, GLint vstride, GLint vorder, GLfixed points){
	    glad_debug_glMap2xOES(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
	}
#endif // glMap2xOES

#ifdef glMapBuffer
#undef glMapBuffer
	static inline void * glMapBuffer(GLenum target, GLenum access){
		return     glad_debug_glMapBuffer(target, access);
	}
#endif // glMapBuffer

#ifdef glMapBufferARB
#undef glMapBufferARB
	static inline void * glMapBufferARB(GLenum target, GLenum access){
		return     glad_debug_glMapBufferARB(target, access);
	}
#endif // glMapBufferARB

#ifdef glMapBufferOES
#undef glMapBufferOES
	static inline void * glMapBufferOES(GLenum target, GLenum access){
		return     glad_debug_glMapBufferOES(target, access);
	}
#endif // glMapBufferOES

#ifdef glMapBufferRange
#undef glMapBufferRange
	static inline void * glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access){
		return     glad_debug_glMapBufferRange(target, offset, length, access);
	}
#endif // glMapBufferRange

#ifdef glMapBufferRangeEXT
#undef glMapBufferRangeEXT
	static inline void * glMapBufferRangeEXT(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access){
		return     glad_debug_glMapBufferRangeEXT(target, offset, length, access);
	}
#endif // glMapBufferRangeEXT

#ifdef glMapGrid1d
#undef glMapGrid1d
	static inline void glMapGrid1d(GLint un, GLdouble u1, GLdouble u2){
	    glad_debug_glMapGrid1d(un, u1, u2);
	}
#endif // glMapGrid1d

#ifdef glMapGrid1f
#undef glMapGrid1f
	static inline void glMapGrid1f(GLint un, GLfloat u1, GLfloat u2){
	    glad_debug_glMapGrid1f(un, u1, u2);
	}
#endif // glMapGrid1f

#ifdef glMapGrid1xOES
#undef glMapGrid1xOES
	static inline void glMapGrid1xOES(GLint n, GLfixed u1, GLfixed u2){
	    glad_debug_glMapGrid1xOES(n, u1, u2);
	}
#endif // glMapGrid1xOES

#ifdef glMapGrid2d
#undef glMapGrid2d
	static inline void glMapGrid2d(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2){
	    glad_debug_glMapGrid2d(un, u1, u2, vn, v1, v2);
	}
#endif // glMapGrid2d

#ifdef glMapGrid2f
#undef glMapGrid2f
	static inline void glMapGrid2f(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2){
	    glad_debug_glMapGrid2f(un, u1, u2, vn, v1, v2);
	}
#endif // glMapGrid2f

#ifdef glMapGrid2xOES
#undef glMapGrid2xOES
	static inline void glMapGrid2xOES(GLint n, GLfixed u1, GLfixed u2, GLfixed v1, GLfixed v2){
	    glad_debug_glMapGrid2xOES(n, u1, u2, v1, v2);
	}
#endif // glMapGrid2xOES

#ifdef glMapNamedBuffer
#undef glMapNamedBuffer
	static inline void * glMapNamedBuffer(GLuint buffer, GLenum access){
		return     glad_debug_glMapNamedBuffer(buffer, access);
	}
#endif // glMapNamedBuffer

#ifdef glMapNamedBufferEXT
#undef glMapNamedBufferEXT
	static inline void * glMapNamedBufferEXT(GLuint buffer, GLenum access){
		return     glad_debug_glMapNamedBufferEXT(buffer, access);
	}
#endif // glMapNamedBufferEXT

#ifdef glMapNamedBufferRange
#undef glMapNamedBufferRange
	static inline void * glMapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access){
		return     glad_debug_glMapNamedBufferRange(buffer, offset, length, access);
	}
#endif // glMapNamedBufferRange

#ifdef glMapNamedBufferRangeEXT
#undef glMapNamedBufferRangeEXT
	static inline void * glMapNamedBufferRangeEXT(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access){
		return     glad_debug_glMapNamedBufferRangeEXT(buffer, offset, length, access);
	}
#endif // glMapNamedBufferRangeEXT

#ifdef glMapObjectBufferATI
#undef glMapObjectBufferATI
	static inline void * glMapObjectBufferATI(GLuint buffer){
		return     glad_debug_glMapObjectBufferATI(buffer);
	}
#endif // glMapObjectBufferATI

#ifdef glMapParameterfvNV
#undef glMapParameterfvNV
	static inline void glMapParameterfvNV(GLenum target, GLenum pname, const GLfloat *params){
	    glad_debug_glMapParameterfvNV(target, pname, params);
	}
#endif // glMapParameterfvNV

#ifdef glMapParameterivNV
#undef glMapParameterivNV
	static inline void glMapParameterivNV(GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glMapParameterivNV(target, pname, params);
	}
#endif // glMapParameterivNV

#ifdef glMapTexture2DINTEL
#undef glMapTexture2DINTEL
	static inline void * glMapTexture2DINTEL(GLuint texture, GLint level, GLbitfield access, GLint *stride, GLenum *layout){
		return     glad_debug_glMapTexture2DINTEL(texture, level, access, stride, layout);
	}
#endif // glMapTexture2DINTEL

#ifdef glMapVertexAttrib1dAPPLE
#undef glMapVertexAttrib1dAPPLE
	static inline void glMapVertexAttrib1dAPPLE(GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points){
	    glad_debug_glMapVertexAttrib1dAPPLE(index, size, u1, u2, stride, order, points);
	}
#endif // glMapVertexAttrib1dAPPLE

#ifdef glMapVertexAttrib1fAPPLE
#undef glMapVertexAttrib1fAPPLE
	static inline void glMapVertexAttrib1fAPPLE(GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points){
	    glad_debug_glMapVertexAttrib1fAPPLE(index, size, u1, u2, stride, order, points);
	}
#endif // glMapVertexAttrib1fAPPLE

#ifdef glMapVertexAttrib2dAPPLE
#undef glMapVertexAttrib2dAPPLE
	static inline void glMapVertexAttrib2dAPPLE(GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points){
	    glad_debug_glMapVertexAttrib2dAPPLE(index, size, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
	}
#endif // glMapVertexAttrib2dAPPLE

#ifdef glMapVertexAttrib2fAPPLE
#undef glMapVertexAttrib2fAPPLE
	static inline void glMapVertexAttrib2fAPPLE(GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points){
	    glad_debug_glMapVertexAttrib2fAPPLE(index, size, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
	}
#endif // glMapVertexAttrib2fAPPLE

#ifdef glMaterialf
#undef glMaterialf
	static inline void glMaterialf(GLenum face, GLenum pname, GLfloat param){
	    glad_debug_glMaterialf(face, pname, param);
	}
#endif // glMaterialf

#ifdef glMaterialfv
#undef glMaterialfv
	static inline void glMaterialfv(GLenum face, GLenum pname, const GLfloat *params){
	    glad_debug_glMaterialfv(face, pname, params);
	}
#endif // glMaterialfv

#ifdef glMateriali
#undef glMateriali
	static inline void glMateriali(GLenum face, GLenum pname, GLint param){
	    glad_debug_glMateriali(face, pname, param);
	}
#endif // glMateriali

#ifdef glMaterialiv
#undef glMaterialiv
	static inline void glMaterialiv(GLenum face, GLenum pname, const GLint *params){
	    glad_debug_glMaterialiv(face, pname, params);
	}
#endif // glMaterialiv

#ifdef glMaterialx
#undef glMaterialx
	static inline void glMaterialx(GLenum face, GLenum pname, GLfixed param){
	    glad_debug_glMaterialx(face, pname, param);
	}
#endif // glMaterialx

#ifdef glMaterialxOES
#undef glMaterialxOES
	static inline void glMaterialxOES(GLenum face, GLenum pname, GLfixed param){
	    glad_debug_glMaterialxOES(face, pname, param);
	}
#endif // glMaterialxOES

#ifdef glMaterialxv
#undef glMaterialxv
	static inline void glMaterialxv(GLenum face, GLenum pname, const GLfixed *param){
	    glad_debug_glMaterialxv(face, pname, param);
	}
#endif // glMaterialxv

#ifdef glMaterialxvOES
#undef glMaterialxvOES
	static inline void glMaterialxvOES(GLenum face, GLenum pname, const GLfixed *param){
	    glad_debug_glMaterialxvOES(face, pname, param);
	}
#endif // glMaterialxvOES

#ifdef glMatrixFrustumEXT
#undef glMatrixFrustumEXT
	static inline void glMatrixFrustumEXT(GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar){
	    glad_debug_glMatrixFrustumEXT(mode, left, right, bottom, top, zNear, zFar);
	}
#endif // glMatrixFrustumEXT

#ifdef glMatrixIndexubvARB
#undef glMatrixIndexubvARB
	static inline void glMatrixIndexubvARB(GLint size, const GLubyte *indices){
	    glad_debug_glMatrixIndexubvARB(size, indices);
	}
#endif // glMatrixIndexubvARB

#ifdef glMatrixIndexuivARB
#undef glMatrixIndexuivARB
	static inline void glMatrixIndexuivARB(GLint size, const GLuint *indices){
	    glad_debug_glMatrixIndexuivARB(size, indices);
	}
#endif // glMatrixIndexuivARB

#ifdef glMatrixIndexusvARB
#undef glMatrixIndexusvARB
	static inline void glMatrixIndexusvARB(GLint size, const GLushort *indices){
	    glad_debug_glMatrixIndexusvARB(size, indices);
	}
#endif // glMatrixIndexusvARB

#ifdef glMatrixLoad3x2fNV
#undef glMatrixLoad3x2fNV
	static inline void glMatrixLoad3x2fNV(GLenum matrixMode, const GLfloat *m){
	    glad_debug_glMatrixLoad3x2fNV(matrixMode, m);
	}
#endif // glMatrixLoad3x2fNV

#ifdef glMatrixLoad3x3fNV
#undef glMatrixLoad3x3fNV
	static inline void glMatrixLoad3x3fNV(GLenum matrixMode, const GLfloat *m){
	    glad_debug_glMatrixLoad3x3fNV(matrixMode, m);
	}
#endif // glMatrixLoad3x3fNV

#ifdef glMatrixLoadIdentityEXT
#undef glMatrixLoadIdentityEXT
	static inline void glMatrixLoadIdentityEXT(GLenum mode){
	    glad_debug_glMatrixLoadIdentityEXT(mode);
	}
#endif // glMatrixLoadIdentityEXT

#ifdef glMatrixLoadTranspose3x3fNV
#undef glMatrixLoadTranspose3x3fNV
	static inline void glMatrixLoadTranspose3x3fNV(GLenum matrixMode, const GLfloat *m){
	    glad_debug_glMatrixLoadTranspose3x3fNV(matrixMode, m);
	}
#endif // glMatrixLoadTranspose3x3fNV

#ifdef glMatrixLoadTransposedEXT
#undef glMatrixLoadTransposedEXT
	static inline void glMatrixLoadTransposedEXT(GLenum mode, const GLdouble *m){
	    glad_debug_glMatrixLoadTransposedEXT(mode, m);
	}
#endif // glMatrixLoadTransposedEXT

#ifdef glMatrixLoadTransposefEXT
#undef glMatrixLoadTransposefEXT
	static inline void glMatrixLoadTransposefEXT(GLenum mode, const GLfloat *m){
	    glad_debug_glMatrixLoadTransposefEXT(mode, m);
	}
#endif // glMatrixLoadTransposefEXT

#ifdef glMatrixLoaddEXT
#undef glMatrixLoaddEXT
	static inline void glMatrixLoaddEXT(GLenum mode, const GLdouble *m){
	    glad_debug_glMatrixLoaddEXT(mode, m);
	}
#endif // glMatrixLoaddEXT

#ifdef glMatrixLoadfEXT
#undef glMatrixLoadfEXT
	static inline void glMatrixLoadfEXT(GLenum mode, const GLfloat *m){
	    glad_debug_glMatrixLoadfEXT(mode, m);
	}
#endif // glMatrixLoadfEXT

#ifdef glMatrixMode
#undef glMatrixMode
	static inline void glMatrixMode(GLenum mode){
	    glad_debug_glMatrixMode(mode);
	}
#endif // glMatrixMode

#ifdef glMatrixMult3x2fNV
#undef glMatrixMult3x2fNV
	static inline void glMatrixMult3x2fNV(GLenum matrixMode, const GLfloat *m){
	    glad_debug_glMatrixMult3x2fNV(matrixMode, m);
	}
#endif // glMatrixMult3x2fNV

#ifdef glMatrixMult3x3fNV
#undef glMatrixMult3x3fNV
	static inline void glMatrixMult3x3fNV(GLenum matrixMode, const GLfloat *m){
	    glad_debug_glMatrixMult3x3fNV(matrixMode, m);
	}
#endif // glMatrixMult3x3fNV

#ifdef glMatrixMultTranspose3x3fNV
#undef glMatrixMultTranspose3x3fNV
	static inline void glMatrixMultTranspose3x3fNV(GLenum matrixMode, const GLfloat *m){
	    glad_debug_glMatrixMultTranspose3x3fNV(matrixMode, m);
	}
#endif // glMatrixMultTranspose3x3fNV

#ifdef glMatrixMultTransposedEXT
#undef glMatrixMultTransposedEXT
	static inline void glMatrixMultTransposedEXT(GLenum mode, const GLdouble *m){
	    glad_debug_glMatrixMultTransposedEXT(mode, m);
	}
#endif // glMatrixMultTransposedEXT

#ifdef glMatrixMultTransposefEXT
#undef glMatrixMultTransposefEXT
	static inline void glMatrixMultTransposefEXT(GLenum mode, const GLfloat *m){
	    glad_debug_glMatrixMultTransposefEXT(mode, m);
	}
#endif // glMatrixMultTransposefEXT

#ifdef glMatrixMultdEXT
#undef glMatrixMultdEXT
	static inline void glMatrixMultdEXT(GLenum mode, const GLdouble *m){
	    glad_debug_glMatrixMultdEXT(mode, m);
	}
#endif // glMatrixMultdEXT

#ifdef glMatrixMultfEXT
#undef glMatrixMultfEXT
	static inline void glMatrixMultfEXT(GLenum mode, const GLfloat *m){
	    glad_debug_glMatrixMultfEXT(mode, m);
	}
#endif // glMatrixMultfEXT

#ifdef glMatrixOrthoEXT
#undef glMatrixOrthoEXT
	static inline void glMatrixOrthoEXT(GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar){
	    glad_debug_glMatrixOrthoEXT(mode, left, right, bottom, top, zNear, zFar);
	}
#endif // glMatrixOrthoEXT

#ifdef glMatrixPopEXT
#undef glMatrixPopEXT
	static inline void glMatrixPopEXT(GLenum mode){
	    glad_debug_glMatrixPopEXT(mode);
	}
#endif // glMatrixPopEXT

#ifdef glMatrixPushEXT
#undef glMatrixPushEXT
	static inline void glMatrixPushEXT(GLenum mode){
	    glad_debug_glMatrixPushEXT(mode);
	}
#endif // glMatrixPushEXT

#ifdef glMatrixRotatedEXT
#undef glMatrixRotatedEXT
	static inline void glMatrixRotatedEXT(GLenum mode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glMatrixRotatedEXT(mode, angle, x, y, z);
	}
#endif // glMatrixRotatedEXT

#ifdef glMatrixRotatefEXT
#undef glMatrixRotatefEXT
	static inline void glMatrixRotatefEXT(GLenum mode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glMatrixRotatefEXT(mode, angle, x, y, z);
	}
#endif // glMatrixRotatefEXT

#ifdef glMatrixScaledEXT
#undef glMatrixScaledEXT
	static inline void glMatrixScaledEXT(GLenum mode, GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glMatrixScaledEXT(mode, x, y, z);
	}
#endif // glMatrixScaledEXT

#ifdef glMatrixScalefEXT
#undef glMatrixScalefEXT
	static inline void glMatrixScalefEXT(GLenum mode, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glMatrixScalefEXT(mode, x, y, z);
	}
#endif // glMatrixScalefEXT

#ifdef glMatrixTranslatedEXT
#undef glMatrixTranslatedEXT
	static inline void glMatrixTranslatedEXT(GLenum mode, GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glMatrixTranslatedEXT(mode, x, y, z);
	}
#endif // glMatrixTranslatedEXT

#ifdef glMatrixTranslatefEXT
#undef glMatrixTranslatefEXT
	static inline void glMatrixTranslatefEXT(GLenum mode, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glMatrixTranslatefEXT(mode, x, y, z);
	}
#endif // glMatrixTranslatefEXT

#ifdef glMaxActiveShaderCoresARM
#undef glMaxActiveShaderCoresARM
	static inline void glMaxActiveShaderCoresARM(GLuint count){
	    glad_debug_glMaxActiveShaderCoresARM(count);
	}
#endif // glMaxActiveShaderCoresARM

#ifdef glMaxShaderCompilerThreadsKHR
#undef glMaxShaderCompilerThreadsKHR
	static inline void glMaxShaderCompilerThreadsKHR(GLuint count){
	    glad_debug_glMaxShaderCompilerThreadsKHR(count);
	}
#endif // glMaxShaderCompilerThreadsKHR

#ifdef glMaxShaderCompilerThreadsARB
#undef glMaxShaderCompilerThreadsARB
	static inline void glMaxShaderCompilerThreadsARB(GLuint count){
	    glad_debug_glMaxShaderCompilerThreadsARB(count);
	}
#endif // glMaxShaderCompilerThreadsARB

#ifdef glMemoryBarrier
#undef glMemoryBarrier
	static inline void glMemoryBarrier(GLbitfield barriers){
	    glad_debug_glMemoryBarrier(barriers);
	}
#endif // glMemoryBarrier

#ifdef glMemoryBarrierByRegion
#undef glMemoryBarrierByRegion
	static inline void glMemoryBarrierByRegion(GLbitfield barriers){
	    glad_debug_glMemoryBarrierByRegion(barriers);
	}
#endif // glMemoryBarrierByRegion

#ifdef glMemoryBarrierEXT
#undef glMemoryBarrierEXT
	static inline void glMemoryBarrierEXT(GLbitfield barriers){
	    glad_debug_glMemoryBarrierEXT(barriers);
	}
#endif // glMemoryBarrierEXT

#ifdef glMemoryObjectParameterivEXT
#undef glMemoryObjectParameterivEXT
	static inline void glMemoryObjectParameterivEXT(GLuint memoryObject, GLenum pname, const GLint *params){
	    glad_debug_glMemoryObjectParameterivEXT(memoryObject, pname, params);
	}
#endif // glMemoryObjectParameterivEXT

#ifdef glMinSampleShading
#undef glMinSampleShading
	static inline void glMinSampleShading(GLfloat value){
	    glad_debug_glMinSampleShading(value);
	}
#endif // glMinSampleShading

#ifdef glMinSampleShadingARB
#undef glMinSampleShadingARB
	static inline void glMinSampleShadingARB(GLfloat value){
	    glad_debug_glMinSampleShadingARB(value);
	}
#endif // glMinSampleShadingARB

#ifdef glMinSampleShadingOES
#undef glMinSampleShadingOES
	static inline void glMinSampleShadingOES(GLfloat value){
	    glad_debug_glMinSampleShadingOES(value);
	}
#endif // glMinSampleShadingOES

#ifdef glMinmax
#undef glMinmax
	static inline void glMinmax(GLenum target, GLenum internalformat, GLboolean sink){
	    glad_debug_glMinmax(target, internalformat, sink);
	}
#endif // glMinmax

#ifdef glMinmaxEXT
#undef glMinmaxEXT
	static inline void glMinmaxEXT(GLenum target, GLenum internalformat, GLboolean sink){
	    glad_debug_glMinmaxEXT(target, internalformat, sink);
	}
#endif // glMinmaxEXT

#ifdef glMultMatrixd
#undef glMultMatrixd
	static inline void glMultMatrixd(const GLdouble *m){
	    glad_debug_glMultMatrixd(m);
	}
#endif // glMultMatrixd

#ifdef glMultMatrixf
#undef glMultMatrixf
	static inline void glMultMatrixf(const GLfloat *m){
	    glad_debug_glMultMatrixf(m);
	}
#endif // glMultMatrixf

#ifdef glMultMatrixx
#undef glMultMatrixx
	static inline void glMultMatrixx(const GLfixed *m){
	    glad_debug_glMultMatrixx(m);
	}
#endif // glMultMatrixx

#ifdef glMultMatrixxOES
#undef glMultMatrixxOES
	static inline void glMultMatrixxOES(const GLfixed *m){
	    glad_debug_glMultMatrixxOES(m);
	}
#endif // glMultMatrixxOES

#ifdef glMultTransposeMatrixd
#undef glMultTransposeMatrixd
	static inline void glMultTransposeMatrixd(const GLdouble *m){
	    glad_debug_glMultTransposeMatrixd(m);
	}
#endif // glMultTransposeMatrixd

#ifdef glMultTransposeMatrixdARB
#undef glMultTransposeMatrixdARB
	static inline void glMultTransposeMatrixdARB(const GLdouble *m){
	    glad_debug_glMultTransposeMatrixdARB(m);
	}
#endif // glMultTransposeMatrixdARB

#ifdef glMultTransposeMatrixf
#undef glMultTransposeMatrixf
	static inline void glMultTransposeMatrixf(const GLfloat *m){
	    glad_debug_glMultTransposeMatrixf(m);
	}
#endif // glMultTransposeMatrixf

#ifdef glMultTransposeMatrixfARB
#undef glMultTransposeMatrixfARB
	static inline void glMultTransposeMatrixfARB(const GLfloat *m){
	    glad_debug_glMultTransposeMatrixfARB(m);
	}
#endif // glMultTransposeMatrixfARB

#ifdef glMultTransposeMatrixxOES
#undef glMultTransposeMatrixxOES
	static inline void glMultTransposeMatrixxOES(const GLfixed *m){
	    glad_debug_glMultTransposeMatrixxOES(m);
	}
#endif // glMultTransposeMatrixxOES

#ifdef glMultiDrawArrays
#undef glMultiDrawArrays
	static inline void glMultiDrawArrays(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount){
	    glad_debug_glMultiDrawArrays(mode, first, count, drawcount);
	}
#endif // glMultiDrawArrays

#ifdef glMultiDrawArraysEXT
#undef glMultiDrawArraysEXT
	static inline void glMultiDrawArraysEXT(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount){
	    glad_debug_glMultiDrawArraysEXT(mode, first, count, primcount);
	}
#endif // glMultiDrawArraysEXT

#ifdef glMultiDrawElementArrayAPPLE
#undef glMultiDrawElementArrayAPPLE
	static inline void glMultiDrawElementArrayAPPLE(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount){
	    glad_debug_glMultiDrawElementArrayAPPLE(mode, first, count, primcount);
	}
#endif // glMultiDrawElementArrayAPPLE

#ifdef glMultiDrawMeshTasksIndirectEXT
#undef glMultiDrawMeshTasksIndirectEXT
	static inline void glMultiDrawMeshTasksIndirectEXT(GLintptr indirect, GLsizei drawcount, GLsizei stride){
	    glad_debug_glMultiDrawMeshTasksIndirectEXT(indirect, drawcount, stride);
	}
#endif // glMultiDrawMeshTasksIndirectEXT

#ifdef glMultiDrawMeshTasksIndirectNV
#undef glMultiDrawMeshTasksIndirectNV
	static inline void glMultiDrawMeshTasksIndirectNV(GLintptr indirect, GLsizei drawcount, GLsizei stride){
	    glad_debug_glMultiDrawMeshTasksIndirectNV(indirect, drawcount, stride);
	}
#endif // glMultiDrawMeshTasksIndirectNV

#ifdef glMultiDrawMeshTasksIndirectCountEXT
#undef glMultiDrawMeshTasksIndirectCountEXT
	static inline void glMultiDrawMeshTasksIndirectCountEXT(GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride){
	    glad_debug_glMultiDrawMeshTasksIndirectCountEXT(indirect, drawcount, maxdrawcount, stride);
	}
#endif // glMultiDrawMeshTasksIndirectCountEXT

#ifdef glMultiDrawMeshTasksIndirectCountNV
#undef glMultiDrawMeshTasksIndirectCountNV
	static inline void glMultiDrawMeshTasksIndirectCountNV(GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride){
	    glad_debug_glMultiDrawMeshTasksIndirectCountNV(indirect, drawcount, maxdrawcount, stride);
	}
#endif // glMultiDrawMeshTasksIndirectCountNV

#ifdef glMultiDrawRangeElementArrayAPPLE
#undef glMultiDrawRangeElementArrayAPPLE
	static inline void glMultiDrawRangeElementArrayAPPLE(GLenum mode, GLuint start, GLuint end, const GLint *first, const GLsizei *count, GLsizei primcount){
	    glad_debug_glMultiDrawRangeElementArrayAPPLE(mode, start, end, first, count, primcount);
	}
#endif // glMultiDrawRangeElementArrayAPPLE

#ifdef glMultiModeDrawArraysIBM
#undef glMultiModeDrawArraysIBM
	static inline void glMultiModeDrawArraysIBM(const GLenum *mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride){
	    glad_debug_glMultiModeDrawArraysIBM(mode, first, count, primcount, modestride);
	}
#endif // glMultiModeDrawArraysIBM

#ifdef glMultiTexBufferEXT
#undef glMultiTexBufferEXT
	static inline void glMultiTexBufferEXT(GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer){
	    glad_debug_glMultiTexBufferEXT(texunit, target, internalformat, buffer);
	}
#endif // glMultiTexBufferEXT

#ifdef glMultiTexCoord1bOES
#undef glMultiTexCoord1bOES
	static inline void glMultiTexCoord1bOES(GLenum texture, GLbyte s){
	    glad_debug_glMultiTexCoord1bOES(texture, s);
	}
#endif // glMultiTexCoord1bOES

#ifdef glMultiTexCoord1bvOES
#undef glMultiTexCoord1bvOES
	static inline void glMultiTexCoord1bvOES(GLenum texture, const GLbyte *coords){
	    glad_debug_glMultiTexCoord1bvOES(texture, coords);
	}
#endif // glMultiTexCoord1bvOES

#ifdef glMultiTexCoord1d
#undef glMultiTexCoord1d
	static inline void glMultiTexCoord1d(GLenum target, GLdouble s){
	    glad_debug_glMultiTexCoord1d(target, s);
	}
#endif // glMultiTexCoord1d

#ifdef glMultiTexCoord1dARB
#undef glMultiTexCoord1dARB
	static inline void glMultiTexCoord1dARB(GLenum target, GLdouble s){
	    glad_debug_glMultiTexCoord1dARB(target, s);
	}
#endif // glMultiTexCoord1dARB

#ifdef glMultiTexCoord1dv
#undef glMultiTexCoord1dv
	static inline void glMultiTexCoord1dv(GLenum target, const GLdouble *v){
	    glad_debug_glMultiTexCoord1dv(target, v);
	}
#endif // glMultiTexCoord1dv

#ifdef glMultiTexCoord1dvARB
#undef glMultiTexCoord1dvARB
	static inline void glMultiTexCoord1dvARB(GLenum target, const GLdouble *v){
	    glad_debug_glMultiTexCoord1dvARB(target, v);
	}
#endif // glMultiTexCoord1dvARB

#ifdef glMultiTexCoord1f
#undef glMultiTexCoord1f
	static inline void glMultiTexCoord1f(GLenum target, GLfloat s){
	    glad_debug_glMultiTexCoord1f(target, s);
	}
#endif // glMultiTexCoord1f

#ifdef glMultiTexCoord1fARB
#undef glMultiTexCoord1fARB
	static inline void glMultiTexCoord1fARB(GLenum target, GLfloat s){
	    glad_debug_glMultiTexCoord1fARB(target, s);
	}
#endif // glMultiTexCoord1fARB

#ifdef glMultiTexCoord1fv
#undef glMultiTexCoord1fv
	static inline void glMultiTexCoord1fv(GLenum target, const GLfloat *v){
	    glad_debug_glMultiTexCoord1fv(target, v);
	}
#endif // glMultiTexCoord1fv

#ifdef glMultiTexCoord1fvARB
#undef glMultiTexCoord1fvARB
	static inline void glMultiTexCoord1fvARB(GLenum target, const GLfloat *v){
	    glad_debug_glMultiTexCoord1fvARB(target, v);
	}
#endif // glMultiTexCoord1fvARB

#ifdef glMultiTexCoord1hNV
#undef glMultiTexCoord1hNV
	static inline void glMultiTexCoord1hNV(GLenum target, GLhalfNV s){
	    glad_debug_glMultiTexCoord1hNV(target, s);
	}
#endif // glMultiTexCoord1hNV

#ifdef glMultiTexCoord1hvNV
#undef glMultiTexCoord1hvNV
	static inline void glMultiTexCoord1hvNV(GLenum target, const GLhalfNV *v){
	    glad_debug_glMultiTexCoord1hvNV(target, v);
	}
#endif // glMultiTexCoord1hvNV

#ifdef glMultiTexCoord1i
#undef glMultiTexCoord1i
	static inline void glMultiTexCoord1i(GLenum target, GLint s){
	    glad_debug_glMultiTexCoord1i(target, s);
	}
#endif // glMultiTexCoord1i

#ifdef glMultiTexCoord1iARB
#undef glMultiTexCoord1iARB
	static inline void glMultiTexCoord1iARB(GLenum target, GLint s){
	    glad_debug_glMultiTexCoord1iARB(target, s);
	}
#endif // glMultiTexCoord1iARB

#ifdef glMultiTexCoord1iv
#undef glMultiTexCoord1iv
	static inline void glMultiTexCoord1iv(GLenum target, const GLint *v){
	    glad_debug_glMultiTexCoord1iv(target, v);
	}
#endif // glMultiTexCoord1iv

#ifdef glMultiTexCoord1ivARB
#undef glMultiTexCoord1ivARB
	static inline void glMultiTexCoord1ivARB(GLenum target, const GLint *v){
	    glad_debug_glMultiTexCoord1ivARB(target, v);
	}
#endif // glMultiTexCoord1ivARB

#ifdef glMultiTexCoord1s
#undef glMultiTexCoord1s
	static inline void glMultiTexCoord1s(GLenum target, GLshort s){
	    glad_debug_glMultiTexCoord1s(target, s);
	}
#endif // glMultiTexCoord1s

#ifdef glMultiTexCoord1sARB
#undef glMultiTexCoord1sARB
	static inline void glMultiTexCoord1sARB(GLenum target, GLshort s){
	    glad_debug_glMultiTexCoord1sARB(target, s);
	}
#endif // glMultiTexCoord1sARB

#ifdef glMultiTexCoord1sv
#undef glMultiTexCoord1sv
	static inline void glMultiTexCoord1sv(GLenum target, const GLshort *v){
	    glad_debug_glMultiTexCoord1sv(target, v);
	}
#endif // glMultiTexCoord1sv

#ifdef glMultiTexCoord1svARB
#undef glMultiTexCoord1svARB
	static inline void glMultiTexCoord1svARB(GLenum target, const GLshort *v){
	    glad_debug_glMultiTexCoord1svARB(target, v);
	}
#endif // glMultiTexCoord1svARB

#ifdef glMultiTexCoord1xOES
#undef glMultiTexCoord1xOES
	static inline void glMultiTexCoord1xOES(GLenum texture, GLfixed s){
	    glad_debug_glMultiTexCoord1xOES(texture, s);
	}
#endif // glMultiTexCoord1xOES

#ifdef glMultiTexCoord1xvOES
#undef glMultiTexCoord1xvOES
	static inline void glMultiTexCoord1xvOES(GLenum texture, const GLfixed *coords){
	    glad_debug_glMultiTexCoord1xvOES(texture, coords);
	}
#endif // glMultiTexCoord1xvOES

#ifdef glMultiTexCoord2bOES
#undef glMultiTexCoord2bOES
	static inline void glMultiTexCoord2bOES(GLenum texture, GLbyte s, GLbyte t){
	    glad_debug_glMultiTexCoord2bOES(texture, s, t);
	}
#endif // glMultiTexCoord2bOES

#ifdef glMultiTexCoord2bvOES
#undef glMultiTexCoord2bvOES
	static inline void glMultiTexCoord2bvOES(GLenum texture, const GLbyte *coords){
	    glad_debug_glMultiTexCoord2bvOES(texture, coords);
	}
#endif // glMultiTexCoord2bvOES

#ifdef glMultiTexCoord2d
#undef glMultiTexCoord2d
	static inline void glMultiTexCoord2d(GLenum target, GLdouble s, GLdouble t){
	    glad_debug_glMultiTexCoord2d(target, s, t);
	}
#endif // glMultiTexCoord2d

#ifdef glMultiTexCoord2dARB
#undef glMultiTexCoord2dARB
	static inline void glMultiTexCoord2dARB(GLenum target, GLdouble s, GLdouble t){
	    glad_debug_glMultiTexCoord2dARB(target, s, t);
	}
#endif // glMultiTexCoord2dARB

#ifdef glMultiTexCoord2dv
#undef glMultiTexCoord2dv
	static inline void glMultiTexCoord2dv(GLenum target, const GLdouble *v){
	    glad_debug_glMultiTexCoord2dv(target, v);
	}
#endif // glMultiTexCoord2dv

#ifdef glMultiTexCoord2dvARB
#undef glMultiTexCoord2dvARB
	static inline void glMultiTexCoord2dvARB(GLenum target, const GLdouble *v){
	    glad_debug_glMultiTexCoord2dvARB(target, v);
	}
#endif // glMultiTexCoord2dvARB

#ifdef glMultiTexCoord2f
#undef glMultiTexCoord2f
	static inline void glMultiTexCoord2f(GLenum target, GLfloat s, GLfloat t){
	    glad_debug_glMultiTexCoord2f(target, s, t);
	}
#endif // glMultiTexCoord2f

#ifdef glMultiTexCoord2fARB
#undef glMultiTexCoord2fARB
	static inline void glMultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t){
	    glad_debug_glMultiTexCoord2fARB(target, s, t);
	}
#endif // glMultiTexCoord2fARB

#ifdef glMultiTexCoord2fv
#undef glMultiTexCoord2fv
	static inline void glMultiTexCoord2fv(GLenum target, const GLfloat *v){
	    glad_debug_glMultiTexCoord2fv(target, v);
	}
#endif // glMultiTexCoord2fv

#ifdef glMultiTexCoord2fvARB
#undef glMultiTexCoord2fvARB
	static inline void glMultiTexCoord2fvARB(GLenum target, const GLfloat *v){
	    glad_debug_glMultiTexCoord2fvARB(target, v);
	}
#endif // glMultiTexCoord2fvARB

#ifdef glMultiTexCoord2hNV
#undef glMultiTexCoord2hNV
	static inline void glMultiTexCoord2hNV(GLenum target, GLhalfNV s, GLhalfNV t){
	    glad_debug_glMultiTexCoord2hNV(target, s, t);
	}
#endif // glMultiTexCoord2hNV

#ifdef glMultiTexCoord2hvNV
#undef glMultiTexCoord2hvNV
	static inline void glMultiTexCoord2hvNV(GLenum target, const GLhalfNV *v){
	    glad_debug_glMultiTexCoord2hvNV(target, v);
	}
#endif // glMultiTexCoord2hvNV

#ifdef glMultiTexCoord2i
#undef glMultiTexCoord2i
	static inline void glMultiTexCoord2i(GLenum target, GLint s, GLint t){
	    glad_debug_glMultiTexCoord2i(target, s, t);
	}
#endif // glMultiTexCoord2i

#ifdef glMultiTexCoord2iARB
#undef glMultiTexCoord2iARB
	static inline void glMultiTexCoord2iARB(GLenum target, GLint s, GLint t){
	    glad_debug_glMultiTexCoord2iARB(target, s, t);
	}
#endif // glMultiTexCoord2iARB

#ifdef glMultiTexCoord2iv
#undef glMultiTexCoord2iv
	static inline void glMultiTexCoord2iv(GLenum target, const GLint *v){
	    glad_debug_glMultiTexCoord2iv(target, v);
	}
#endif // glMultiTexCoord2iv

#ifdef glMultiTexCoord2ivARB
#undef glMultiTexCoord2ivARB
	static inline void glMultiTexCoord2ivARB(GLenum target, const GLint *v){
	    glad_debug_glMultiTexCoord2ivARB(target, v);
	}
#endif // glMultiTexCoord2ivARB

#ifdef glMultiTexCoord2s
#undef glMultiTexCoord2s
	static inline void glMultiTexCoord2s(GLenum target, GLshort s, GLshort t){
	    glad_debug_glMultiTexCoord2s(target, s, t);
	}
#endif // glMultiTexCoord2s

#ifdef glMultiTexCoord2sARB
#undef glMultiTexCoord2sARB
	static inline void glMultiTexCoord2sARB(GLenum target, GLshort s, GLshort t){
	    glad_debug_glMultiTexCoord2sARB(target, s, t);
	}
#endif // glMultiTexCoord2sARB

#ifdef glMultiTexCoord2sv
#undef glMultiTexCoord2sv
	static inline void glMultiTexCoord2sv(GLenum target, const GLshort *v){
	    glad_debug_glMultiTexCoord2sv(target, v);
	}
#endif // glMultiTexCoord2sv

#ifdef glMultiTexCoord2svARB
#undef glMultiTexCoord2svARB
	static inline void glMultiTexCoord2svARB(GLenum target, const GLshort *v){
	    glad_debug_glMultiTexCoord2svARB(target, v);
	}
#endif // glMultiTexCoord2svARB

#ifdef glMultiTexCoord2xOES
#undef glMultiTexCoord2xOES
	static inline void glMultiTexCoord2xOES(GLenum texture, GLfixed s, GLfixed t){
	    glad_debug_glMultiTexCoord2xOES(texture, s, t);
	}
#endif // glMultiTexCoord2xOES

#ifdef glMultiTexCoord2xvOES
#undef glMultiTexCoord2xvOES
	static inline void glMultiTexCoord2xvOES(GLenum texture, const GLfixed *coords){
	    glad_debug_glMultiTexCoord2xvOES(texture, coords);
	}
#endif // glMultiTexCoord2xvOES

#ifdef glMultiTexCoord3bOES
#undef glMultiTexCoord3bOES
	static inline void glMultiTexCoord3bOES(GLenum texture, GLbyte s, GLbyte t, GLbyte r){
	    glad_debug_glMultiTexCoord3bOES(texture, s, t, r);
	}
#endif // glMultiTexCoord3bOES

#ifdef glMultiTexCoord3bvOES
#undef glMultiTexCoord3bvOES
	static inline void glMultiTexCoord3bvOES(GLenum texture, const GLbyte *coords){
	    glad_debug_glMultiTexCoord3bvOES(texture, coords);
	}
#endif // glMultiTexCoord3bvOES

#ifdef glMultiTexCoord3d
#undef glMultiTexCoord3d
	static inline void glMultiTexCoord3d(GLenum target, GLdouble s, GLdouble t, GLdouble r){
	    glad_debug_glMultiTexCoord3d(target, s, t, r);
	}
#endif // glMultiTexCoord3d

#ifdef glMultiTexCoord3dARB
#undef glMultiTexCoord3dARB
	static inline void glMultiTexCoord3dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r){
	    glad_debug_glMultiTexCoord3dARB(target, s, t, r);
	}
#endif // glMultiTexCoord3dARB

#ifdef glMultiTexCoord3dv
#undef glMultiTexCoord3dv
	static inline void glMultiTexCoord3dv(GLenum target, const GLdouble *v){
	    glad_debug_glMultiTexCoord3dv(target, v);
	}
#endif // glMultiTexCoord3dv

#ifdef glMultiTexCoord3dvARB
#undef glMultiTexCoord3dvARB
	static inline void glMultiTexCoord3dvARB(GLenum target, const GLdouble *v){
	    glad_debug_glMultiTexCoord3dvARB(target, v);
	}
#endif // glMultiTexCoord3dvARB

#ifdef glMultiTexCoord3f
#undef glMultiTexCoord3f
	static inline void glMultiTexCoord3f(GLenum target, GLfloat s, GLfloat t, GLfloat r){
	    glad_debug_glMultiTexCoord3f(target, s, t, r);
	}
#endif // glMultiTexCoord3f

#ifdef glMultiTexCoord3fARB
#undef glMultiTexCoord3fARB
	static inline void glMultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r){
	    glad_debug_glMultiTexCoord3fARB(target, s, t, r);
	}
#endif // glMultiTexCoord3fARB

#ifdef glMultiTexCoord3fv
#undef glMultiTexCoord3fv
	static inline void glMultiTexCoord3fv(GLenum target, const GLfloat *v){
	    glad_debug_glMultiTexCoord3fv(target, v);
	}
#endif // glMultiTexCoord3fv

#ifdef glMultiTexCoord3fvARB
#undef glMultiTexCoord3fvARB
	static inline void glMultiTexCoord3fvARB(GLenum target, const GLfloat *v){
	    glad_debug_glMultiTexCoord3fvARB(target, v);
	}
#endif // glMultiTexCoord3fvARB

#ifdef glMultiTexCoord3hNV
#undef glMultiTexCoord3hNV
	static inline void glMultiTexCoord3hNV(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r){
	    glad_debug_glMultiTexCoord3hNV(target, s, t, r);
	}
#endif // glMultiTexCoord3hNV

#ifdef glMultiTexCoord3hvNV
#undef glMultiTexCoord3hvNV
	static inline void glMultiTexCoord3hvNV(GLenum target, const GLhalfNV *v){
	    glad_debug_glMultiTexCoord3hvNV(target, v);
	}
#endif // glMultiTexCoord3hvNV

#ifdef glMultiTexCoord3i
#undef glMultiTexCoord3i
	static inline void glMultiTexCoord3i(GLenum target, GLint s, GLint t, GLint r){
	    glad_debug_glMultiTexCoord3i(target, s, t, r);
	}
#endif // glMultiTexCoord3i

#ifdef glMultiTexCoord3iARB
#undef glMultiTexCoord3iARB
	static inline void glMultiTexCoord3iARB(GLenum target, GLint s, GLint t, GLint r){
	    glad_debug_glMultiTexCoord3iARB(target, s, t, r);
	}
#endif // glMultiTexCoord3iARB

#ifdef glMultiTexCoord3iv
#undef glMultiTexCoord3iv
	static inline void glMultiTexCoord3iv(GLenum target, const GLint *v){
	    glad_debug_glMultiTexCoord3iv(target, v);
	}
#endif // glMultiTexCoord3iv

#ifdef glMultiTexCoord3ivARB
#undef glMultiTexCoord3ivARB
	static inline void glMultiTexCoord3ivARB(GLenum target, const GLint *v){
	    glad_debug_glMultiTexCoord3ivARB(target, v);
	}
#endif // glMultiTexCoord3ivARB

#ifdef glMultiTexCoord3s
#undef glMultiTexCoord3s
	static inline void glMultiTexCoord3s(GLenum target, GLshort s, GLshort t, GLshort r){
	    glad_debug_glMultiTexCoord3s(target, s, t, r);
	}
#endif // glMultiTexCoord3s

#ifdef glMultiTexCoord3sARB
#undef glMultiTexCoord3sARB
	static inline void glMultiTexCoord3sARB(GLenum target, GLshort s, GLshort t, GLshort r){
	    glad_debug_glMultiTexCoord3sARB(target, s, t, r);
	}
#endif // glMultiTexCoord3sARB

#ifdef glMultiTexCoord3sv
#undef glMultiTexCoord3sv
	static inline void glMultiTexCoord3sv(GLenum target, const GLshort *v){
	    glad_debug_glMultiTexCoord3sv(target, v);
	}
#endif // glMultiTexCoord3sv

#ifdef glMultiTexCoord3svARB
#undef glMultiTexCoord3svARB
	static inline void glMultiTexCoord3svARB(GLenum target, const GLshort *v){
	    glad_debug_glMultiTexCoord3svARB(target, v);
	}
#endif // glMultiTexCoord3svARB

#ifdef glMultiTexCoord3xOES
#undef glMultiTexCoord3xOES
	static inline void glMultiTexCoord3xOES(GLenum texture, GLfixed s, GLfixed t, GLfixed r){
	    glad_debug_glMultiTexCoord3xOES(texture, s, t, r);
	}
#endif // glMultiTexCoord3xOES

#ifdef glMultiTexCoord3xvOES
#undef glMultiTexCoord3xvOES
	static inline void glMultiTexCoord3xvOES(GLenum texture, const GLfixed *coords){
	    glad_debug_glMultiTexCoord3xvOES(texture, coords);
	}
#endif // glMultiTexCoord3xvOES

#ifdef glMultiTexCoord4bOES
#undef glMultiTexCoord4bOES
	static inline void glMultiTexCoord4bOES(GLenum texture, GLbyte s, GLbyte t, GLbyte r, GLbyte q){
	    glad_debug_glMultiTexCoord4bOES(texture, s, t, r, q);
	}
#endif // glMultiTexCoord4bOES

#ifdef glMultiTexCoord4bvOES
#undef glMultiTexCoord4bvOES
	static inline void glMultiTexCoord4bvOES(GLenum texture, const GLbyte *coords){
	    glad_debug_glMultiTexCoord4bvOES(texture, coords);
	}
#endif // glMultiTexCoord4bvOES

#ifdef glMultiTexCoord4d
#undef glMultiTexCoord4d
	static inline void glMultiTexCoord4d(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q){
	    glad_debug_glMultiTexCoord4d(target, s, t, r, q);
	}
#endif // glMultiTexCoord4d

#ifdef glMultiTexCoord4dARB
#undef glMultiTexCoord4dARB
	static inline void glMultiTexCoord4dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q){
	    glad_debug_glMultiTexCoord4dARB(target, s, t, r, q);
	}
#endif // glMultiTexCoord4dARB

#ifdef glMultiTexCoord4dv
#undef glMultiTexCoord4dv
	static inline void glMultiTexCoord4dv(GLenum target, const GLdouble *v){
	    glad_debug_glMultiTexCoord4dv(target, v);
	}
#endif // glMultiTexCoord4dv

#ifdef glMultiTexCoord4dvARB
#undef glMultiTexCoord4dvARB
	static inline void glMultiTexCoord4dvARB(GLenum target, const GLdouble *v){
	    glad_debug_glMultiTexCoord4dvARB(target, v);
	}
#endif // glMultiTexCoord4dvARB

#ifdef glMultiTexCoord4f
#undef glMultiTexCoord4f
	static inline void glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q){
	    glad_debug_glMultiTexCoord4f(target, s, t, r, q);
	}
#endif // glMultiTexCoord4f

#ifdef glMultiTexCoord4fARB
#undef glMultiTexCoord4fARB
	static inline void glMultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q){
	    glad_debug_glMultiTexCoord4fARB(target, s, t, r, q);
	}
#endif // glMultiTexCoord4fARB

#ifdef glMultiTexCoord4fv
#undef glMultiTexCoord4fv
	static inline void glMultiTexCoord4fv(GLenum target, const GLfloat *v){
	    glad_debug_glMultiTexCoord4fv(target, v);
	}
#endif // glMultiTexCoord4fv

#ifdef glMultiTexCoord4fvARB
#undef glMultiTexCoord4fvARB
	static inline void glMultiTexCoord4fvARB(GLenum target, const GLfloat *v){
	    glad_debug_glMultiTexCoord4fvARB(target, v);
	}
#endif // glMultiTexCoord4fvARB

#ifdef glMultiTexCoord4hNV
#undef glMultiTexCoord4hNV
	static inline void glMultiTexCoord4hNV(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q){
	    glad_debug_glMultiTexCoord4hNV(target, s, t, r, q);
	}
#endif // glMultiTexCoord4hNV

#ifdef glMultiTexCoord4hvNV
#undef glMultiTexCoord4hvNV
	static inline void glMultiTexCoord4hvNV(GLenum target, const GLhalfNV *v){
	    glad_debug_glMultiTexCoord4hvNV(target, v);
	}
#endif // glMultiTexCoord4hvNV

#ifdef glMultiTexCoord4i
#undef glMultiTexCoord4i
	static inline void glMultiTexCoord4i(GLenum target, GLint s, GLint t, GLint r, GLint q){
	    glad_debug_glMultiTexCoord4i(target, s, t, r, q);
	}
#endif // glMultiTexCoord4i

#ifdef glMultiTexCoord4iARB
#undef glMultiTexCoord4iARB
	static inline void glMultiTexCoord4iARB(GLenum target, GLint s, GLint t, GLint r, GLint q){
	    glad_debug_glMultiTexCoord4iARB(target, s, t, r, q);
	}
#endif // glMultiTexCoord4iARB

#ifdef glMultiTexCoord4iv
#undef glMultiTexCoord4iv
	static inline void glMultiTexCoord4iv(GLenum target, const GLint *v){
	    glad_debug_glMultiTexCoord4iv(target, v);
	}
#endif // glMultiTexCoord4iv

#ifdef glMultiTexCoord4ivARB
#undef glMultiTexCoord4ivARB
	static inline void glMultiTexCoord4ivARB(GLenum target, const GLint *v){
	    glad_debug_glMultiTexCoord4ivARB(target, v);
	}
#endif // glMultiTexCoord4ivARB

#ifdef glMultiTexCoord4s
#undef glMultiTexCoord4s
	static inline void glMultiTexCoord4s(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q){
	    glad_debug_glMultiTexCoord4s(target, s, t, r, q);
	}
#endif // glMultiTexCoord4s

#ifdef glMultiTexCoord4sARB
#undef glMultiTexCoord4sARB
	static inline void glMultiTexCoord4sARB(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q){
	    glad_debug_glMultiTexCoord4sARB(target, s, t, r, q);
	}
#endif // glMultiTexCoord4sARB

#ifdef glMultiTexCoord4sv
#undef glMultiTexCoord4sv
	static inline void glMultiTexCoord4sv(GLenum target, const GLshort *v){
	    glad_debug_glMultiTexCoord4sv(target, v);
	}
#endif // glMultiTexCoord4sv

#ifdef glMultiTexCoord4svARB
#undef glMultiTexCoord4svARB
	static inline void glMultiTexCoord4svARB(GLenum target, const GLshort *v){
	    glad_debug_glMultiTexCoord4svARB(target, v);
	}
#endif // glMultiTexCoord4svARB

#ifdef glMultiTexCoord4x
#undef glMultiTexCoord4x
	static inline void glMultiTexCoord4x(GLenum texture, GLfixed s, GLfixed t, GLfixed r, GLfixed q){
	    glad_debug_glMultiTexCoord4x(texture, s, t, r, q);
	}
#endif // glMultiTexCoord4x

#ifdef glMultiTexCoord4xOES
#undef glMultiTexCoord4xOES
	static inline void glMultiTexCoord4xOES(GLenum texture, GLfixed s, GLfixed t, GLfixed r, GLfixed q){
	    glad_debug_glMultiTexCoord4xOES(texture, s, t, r, q);
	}
#endif // glMultiTexCoord4xOES

#ifdef glMultiTexCoord4xvOES
#undef glMultiTexCoord4xvOES
	static inline void glMultiTexCoord4xvOES(GLenum texture, const GLfixed *coords){
	    glad_debug_glMultiTexCoord4xvOES(texture, coords);
	}
#endif // glMultiTexCoord4xvOES

#ifdef glMultiTexCoordP1ui
#undef glMultiTexCoordP1ui
	static inline void glMultiTexCoordP1ui(GLenum texture, GLenum type, GLuint coords){
	    glad_debug_glMultiTexCoordP1ui(texture, type, coords);
	}
#endif // glMultiTexCoordP1ui

#ifdef glMultiTexCoordP1uiv
#undef glMultiTexCoordP1uiv
	static inline void glMultiTexCoordP1uiv(GLenum texture, GLenum type, const GLuint *coords){
	    glad_debug_glMultiTexCoordP1uiv(texture, type, coords);
	}
#endif // glMultiTexCoordP1uiv

#ifdef glMultiTexCoordP2ui
#undef glMultiTexCoordP2ui
	static inline void glMultiTexCoordP2ui(GLenum texture, GLenum type, GLuint coords){
	    glad_debug_glMultiTexCoordP2ui(texture, type, coords);
	}
#endif // glMultiTexCoordP2ui

#ifdef glMultiTexCoordP2uiv
#undef glMultiTexCoordP2uiv
	static inline void glMultiTexCoordP2uiv(GLenum texture, GLenum type, const GLuint *coords){
	    glad_debug_glMultiTexCoordP2uiv(texture, type, coords);
	}
#endif // glMultiTexCoordP2uiv

#ifdef glMultiTexCoordP3ui
#undef glMultiTexCoordP3ui
	static inline void glMultiTexCoordP3ui(GLenum texture, GLenum type, GLuint coords){
	    glad_debug_glMultiTexCoordP3ui(texture, type, coords);
	}
#endif // glMultiTexCoordP3ui

#ifdef glMultiTexCoordP3uiv
#undef glMultiTexCoordP3uiv
	static inline void glMultiTexCoordP3uiv(GLenum texture, GLenum type, const GLuint *coords){
	    glad_debug_glMultiTexCoordP3uiv(texture, type, coords);
	}
#endif // glMultiTexCoordP3uiv

#ifdef glMultiTexCoordP4ui
#undef glMultiTexCoordP4ui
	static inline void glMultiTexCoordP4ui(GLenum texture, GLenum type, GLuint coords){
	    glad_debug_glMultiTexCoordP4ui(texture, type, coords);
	}
#endif // glMultiTexCoordP4ui

#ifdef glMultiTexCoordP4uiv
#undef glMultiTexCoordP4uiv
	static inline void glMultiTexCoordP4uiv(GLenum texture, GLenum type, const GLuint *coords){
	    glad_debug_glMultiTexCoordP4uiv(texture, type, coords);
	}
#endif // glMultiTexCoordP4uiv

#ifdef glMultiTexEnvfEXT
#undef glMultiTexEnvfEXT
	static inline void glMultiTexEnvfEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat param){
	    glad_debug_glMultiTexEnvfEXT(texunit, target, pname, param);
	}
#endif // glMultiTexEnvfEXT

#ifdef glMultiTexEnvfvEXT
#undef glMultiTexEnvfvEXT
	static inline void glMultiTexEnvfvEXT(GLenum texunit, GLenum target, GLenum pname, const GLfloat *params){
	    glad_debug_glMultiTexEnvfvEXT(texunit, target, pname, params);
	}
#endif // glMultiTexEnvfvEXT

#ifdef glMultiTexEnviEXT
#undef glMultiTexEnviEXT
	static inline void glMultiTexEnviEXT(GLenum texunit, GLenum target, GLenum pname, GLint param){
	    glad_debug_glMultiTexEnviEXT(texunit, target, pname, param);
	}
#endif // glMultiTexEnviEXT

#ifdef glMultiTexEnvivEXT
#undef glMultiTexEnvivEXT
	static inline void glMultiTexEnvivEXT(GLenum texunit, GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glMultiTexEnvivEXT(texunit, target, pname, params);
	}
#endif // glMultiTexEnvivEXT

#ifdef glMultiTexGendEXT
#undef glMultiTexGendEXT
	static inline void glMultiTexGendEXT(GLenum texunit, GLenum coord, GLenum pname, GLdouble param){
	    glad_debug_glMultiTexGendEXT(texunit, coord, pname, param);
	}
#endif // glMultiTexGendEXT

#ifdef glMultiTexGendvEXT
#undef glMultiTexGendvEXT
	static inline void glMultiTexGendvEXT(GLenum texunit, GLenum coord, GLenum pname, const GLdouble *params){
	    glad_debug_glMultiTexGendvEXT(texunit, coord, pname, params);
	}
#endif // glMultiTexGendvEXT

#ifdef glMultiTexGenfEXT
#undef glMultiTexGenfEXT
	static inline void glMultiTexGenfEXT(GLenum texunit, GLenum coord, GLenum pname, GLfloat param){
	    glad_debug_glMultiTexGenfEXT(texunit, coord, pname, param);
	}
#endif // glMultiTexGenfEXT

#ifdef glMultiTexGenfvEXT
#undef glMultiTexGenfvEXT
	static inline void glMultiTexGenfvEXT(GLenum texunit, GLenum coord, GLenum pname, const GLfloat *params){
	    glad_debug_glMultiTexGenfvEXT(texunit, coord, pname, params);
	}
#endif // glMultiTexGenfvEXT

#ifdef glMultiTexGeniEXT
#undef glMultiTexGeniEXT
	static inline void glMultiTexGeniEXT(GLenum texunit, GLenum coord, GLenum pname, GLint param){
	    glad_debug_glMultiTexGeniEXT(texunit, coord, pname, param);
	}
#endif // glMultiTexGeniEXT

#ifdef glMultiTexGenivEXT
#undef glMultiTexGenivEXT
	static inline void glMultiTexGenivEXT(GLenum texunit, GLenum coord, GLenum pname, const GLint *params){
	    glad_debug_glMultiTexGenivEXT(texunit, coord, pname, params);
	}
#endif // glMultiTexGenivEXT

#ifdef glMultiTexParameterIivEXT
#undef glMultiTexParameterIivEXT
	static inline void glMultiTexParameterIivEXT(GLenum texunit, GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glMultiTexParameterIivEXT(texunit, target, pname, params);
	}
#endif // glMultiTexParameterIivEXT

#ifdef glMultiTexParameterIuivEXT
#undef glMultiTexParameterIuivEXT
	static inline void glMultiTexParameterIuivEXT(GLenum texunit, GLenum target, GLenum pname, const GLuint *params){
	    glad_debug_glMultiTexParameterIuivEXT(texunit, target, pname, params);
	}
#endif // glMultiTexParameterIuivEXT

#ifdef glMultiTexParameterfEXT
#undef glMultiTexParameterfEXT
	static inline void glMultiTexParameterfEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat param){
	    glad_debug_glMultiTexParameterfEXT(texunit, target, pname, param);
	}
#endif // glMultiTexParameterfEXT

#ifdef glMultiTexParameterfvEXT
#undef glMultiTexParameterfvEXT
	static inline void glMultiTexParameterfvEXT(GLenum texunit, GLenum target, GLenum pname, const GLfloat *params){
	    glad_debug_glMultiTexParameterfvEXT(texunit, target, pname, params);
	}
#endif // glMultiTexParameterfvEXT

#ifdef glMultiTexParameteriEXT
#undef glMultiTexParameteriEXT
	static inline void glMultiTexParameteriEXT(GLenum texunit, GLenum target, GLenum pname, GLint param){
	    glad_debug_glMultiTexParameteriEXT(texunit, target, pname, param);
	}
#endif // glMultiTexParameteriEXT

#ifdef glMultiTexParameterivEXT
#undef glMultiTexParameterivEXT
	static inline void glMultiTexParameterivEXT(GLenum texunit, GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glMultiTexParameterivEXT(texunit, target, pname, params);
	}
#endif // glMultiTexParameterivEXT

#ifdef glMultiTexRenderbufferEXT
#undef glMultiTexRenderbufferEXT
	static inline void glMultiTexRenderbufferEXT(GLenum texunit, GLenum target, GLuint renderbuffer){
	    glad_debug_glMultiTexRenderbufferEXT(texunit, target, renderbuffer);
	}
#endif // glMultiTexRenderbufferEXT

#ifdef glMulticastBarrierNV
#undef glMulticastBarrierNV
	static inline void glMulticastBarrierNV(){
	    glad_debug_glMulticastBarrierNV();
	}
#endif // glMulticastBarrierNV

#ifdef glMulticastBlitFramebufferNV
#undef glMulticastBlitFramebufferNV
	static inline void glMulticastBlitFramebufferNV(GLuint srcGpu, GLuint dstGpu, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter){
	    glad_debug_glMulticastBlitFramebufferNV(srcGpu, dstGpu, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	}
#endif // glMulticastBlitFramebufferNV

#ifdef glMulticastCopyBufferSubDataNV
#undef glMulticastCopyBufferSubDataNV
	static inline void glMulticastCopyBufferSubDataNV(GLuint readGpu, GLbitfield writeGpuMask, GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size){
	    glad_debug_glMulticastCopyBufferSubDataNV(readGpu, writeGpuMask, readBuffer, writeBuffer, readOffset, writeOffset, size);
	}
#endif // glMulticastCopyBufferSubDataNV

#ifdef glMulticastCopyImageSubDataNV
#undef glMulticastCopyImageSubDataNV
	static inline void glMulticastCopyImageSubDataNV(GLuint srcGpu, GLbitfield dstGpuMask, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth){
	    glad_debug_glMulticastCopyImageSubDataNV(srcGpu, dstGpuMask, srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);
	}
#endif // glMulticastCopyImageSubDataNV

#ifdef glMulticastFramebufferSampleLocationsfvNV
#undef glMulticastFramebufferSampleLocationsfvNV
	static inline void glMulticastFramebufferSampleLocationsfvNV(GLuint gpu, GLuint framebuffer, GLuint start, GLsizei count, const GLfloat *v){
	    glad_debug_glMulticastFramebufferSampleLocationsfvNV(gpu, framebuffer, start, count, v);
	}
#endif // glMulticastFramebufferSampleLocationsfvNV

#ifdef glMulticastGetQueryObjecti64vNV
#undef glMulticastGetQueryObjecti64vNV
	static inline void glMulticastGetQueryObjecti64vNV(GLuint gpu, GLuint id, GLenum pname, GLint64 *params){
	    glad_debug_glMulticastGetQueryObjecti64vNV(gpu, id, pname, params);
	}
#endif // glMulticastGetQueryObjecti64vNV

#ifdef glMulticastGetQueryObjectivNV
#undef glMulticastGetQueryObjectivNV
	static inline void glMulticastGetQueryObjectivNV(GLuint gpu, GLuint id, GLenum pname, GLint *params){
	    glad_debug_glMulticastGetQueryObjectivNV(gpu, id, pname, params);
	}
#endif // glMulticastGetQueryObjectivNV

#ifdef glMulticastGetQueryObjectui64vNV
#undef glMulticastGetQueryObjectui64vNV
	static inline void glMulticastGetQueryObjectui64vNV(GLuint gpu, GLuint id, GLenum pname, GLuint64 *params){
	    glad_debug_glMulticastGetQueryObjectui64vNV(gpu, id, pname, params);
	}
#endif // glMulticastGetQueryObjectui64vNV

#ifdef glMulticastGetQueryObjectuivNV
#undef glMulticastGetQueryObjectuivNV
	static inline void glMulticastGetQueryObjectuivNV(GLuint gpu, GLuint id, GLenum pname, GLuint *params){
	    glad_debug_glMulticastGetQueryObjectuivNV(gpu, id, pname, params);
	}
#endif // glMulticastGetQueryObjectuivNV

#ifdef glMulticastScissorArrayvNVX
#undef glMulticastScissorArrayvNVX
	static inline void glMulticastScissorArrayvNVX(GLuint gpu, GLuint first, GLsizei count, const GLint *v){
	    glad_debug_glMulticastScissorArrayvNVX(gpu, first, count, v);
	}
#endif // glMulticastScissorArrayvNVX

#ifdef glMulticastViewportArrayvNVX
#undef glMulticastViewportArrayvNVX
	static inline void glMulticastViewportArrayvNVX(GLuint gpu, GLuint first, GLsizei count, const GLfloat *v){
	    glad_debug_glMulticastViewportArrayvNVX(gpu, first, count, v);
	}
#endif // glMulticastViewportArrayvNVX

#ifdef glMulticastViewportPositionWScaleNVX
#undef glMulticastViewportPositionWScaleNVX
	static inline void glMulticastViewportPositionWScaleNVX(GLuint gpu, GLuint index, GLfloat xcoeff, GLfloat ycoeff){
	    glad_debug_glMulticastViewportPositionWScaleNVX(gpu, index, xcoeff, ycoeff);
	}
#endif // glMulticastViewportPositionWScaleNVX

#ifdef glMulticastWaitSyncNV
#undef glMulticastWaitSyncNV
	static inline void glMulticastWaitSyncNV(GLuint signalGpu, GLbitfield waitGpuMask){
	    glad_debug_glMulticastWaitSyncNV(signalGpu, waitGpuMask);
	}
#endif // glMulticastWaitSyncNV

#ifdef glNamedBufferAttachMemoryNV
#undef glNamedBufferAttachMemoryNV
	static inline void glNamedBufferAttachMemoryNV(GLuint buffer, GLuint memory, GLuint64 offset){
	    glad_debug_glNamedBufferAttachMemoryNV(buffer, memory, offset);
	}
#endif // glNamedBufferAttachMemoryNV

#ifdef glNamedBufferPageCommitmentARB
#undef glNamedBufferPageCommitmentARB
	static inline void glNamedBufferPageCommitmentARB(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit){
	    glad_debug_glNamedBufferPageCommitmentARB(buffer, offset, size, commit);
	}
#endif // glNamedBufferPageCommitmentARB

#ifdef glNamedBufferPageCommitmentEXT
#undef glNamedBufferPageCommitmentEXT
	static inline void glNamedBufferPageCommitmentEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit){
	    glad_debug_glNamedBufferPageCommitmentEXT(buffer, offset, size, commit);
	}
#endif // glNamedBufferPageCommitmentEXT

#ifdef glNamedBufferPageCommitmentMemNV
#undef glNamedBufferPageCommitmentMemNV
	static inline void glNamedBufferPageCommitmentMemNV(GLuint buffer, GLintptr offset, GLsizeiptr size, GLuint memory, GLuint64 memOffset, GLboolean commit){
	    glad_debug_glNamedBufferPageCommitmentMemNV(buffer, offset, size, memory, memOffset, commit);
	}
#endif // glNamedBufferPageCommitmentMemNV

#ifdef glNamedBufferStorageExternalEXT
#undef glNamedBufferStorageExternalEXT
	static inline void glNamedBufferStorageExternalEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, GLeglClientBufferEXT clientBuffer, GLbitfield flags){
	    glad_debug_glNamedBufferStorageExternalEXT(buffer, offset, size, clientBuffer, flags);
	}
#endif // glNamedBufferStorageExternalEXT

#ifdef glNamedBufferStorageMemEXT
#undef glNamedBufferStorageMemEXT
	static inline void glNamedBufferStorageMemEXT(GLuint buffer, GLsizeiptr size, GLuint memory, GLuint64 offset){
	    glad_debug_glNamedBufferStorageMemEXT(buffer, size, memory, offset);
	}
#endif // glNamedBufferStorageMemEXT

#ifdef glNamedCopyBufferSubDataEXT
#undef glNamedCopyBufferSubDataEXT
	static inline void glNamedCopyBufferSubDataEXT(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size){
	    glad_debug_glNamedCopyBufferSubDataEXT(readBuffer, writeBuffer, readOffset, writeOffset, size);
	}
#endif // glNamedCopyBufferSubDataEXT

#ifdef glNamedFramebufferDrawBuffer
#undef glNamedFramebufferDrawBuffer
	static inline void glNamedFramebufferDrawBuffer(GLuint framebuffer, GLenum buf){
	    glad_debug_glNamedFramebufferDrawBuffer(framebuffer, buf);
	}
#endif // glNamedFramebufferDrawBuffer

#ifdef glNamedFramebufferDrawBuffers
#undef glNamedFramebufferDrawBuffers
	static inline void glNamedFramebufferDrawBuffers(GLuint framebuffer, GLsizei n, const GLenum *bufs){
	    glad_debug_glNamedFramebufferDrawBuffers(framebuffer, n, bufs);
	}
#endif // glNamedFramebufferDrawBuffers

#ifdef glNamedFramebufferParameteri
#undef glNamedFramebufferParameteri
	static inline void glNamedFramebufferParameteri(GLuint framebuffer, GLenum pname, GLint param){
	    glad_debug_glNamedFramebufferParameteri(framebuffer, pname, param);
	}
#endif // glNamedFramebufferParameteri

#ifdef glNamedFramebufferParameteriEXT
#undef glNamedFramebufferParameteriEXT
	static inline void glNamedFramebufferParameteriEXT(GLuint framebuffer, GLenum pname, GLint param){
	    glad_debug_glNamedFramebufferParameteriEXT(framebuffer, pname, param);
	}
#endif // glNamedFramebufferParameteriEXT

#ifdef glNamedFramebufferReadBuffer
#undef glNamedFramebufferReadBuffer
	static inline void glNamedFramebufferReadBuffer(GLuint framebuffer, GLenum src){
	    glad_debug_glNamedFramebufferReadBuffer(framebuffer, src);
	}
#endif // glNamedFramebufferReadBuffer

#ifdef glNamedFramebufferRenderbuffer
#undef glNamedFramebufferRenderbuffer
	static inline void glNamedFramebufferRenderbuffer(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer){
	    glad_debug_glNamedFramebufferRenderbuffer(framebuffer, attachment, renderbuffertarget, renderbuffer);
	}
#endif // glNamedFramebufferRenderbuffer

#ifdef glNamedFramebufferRenderbufferEXT
#undef glNamedFramebufferRenderbufferEXT
	static inline void glNamedFramebufferRenderbufferEXT(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer){
	    glad_debug_glNamedFramebufferRenderbufferEXT(framebuffer, attachment, renderbuffertarget, renderbuffer);
	}
#endif // glNamedFramebufferRenderbufferEXT

#ifdef glNamedFramebufferSampleLocationsfvARB
#undef glNamedFramebufferSampleLocationsfvARB
	static inline void glNamedFramebufferSampleLocationsfvARB(GLuint framebuffer, GLuint start, GLsizei count, const GLfloat *v){
	    glad_debug_glNamedFramebufferSampleLocationsfvARB(framebuffer, start, count, v);
	}
#endif // glNamedFramebufferSampleLocationsfvARB

#ifdef glNamedFramebufferSampleLocationsfvNV
#undef glNamedFramebufferSampleLocationsfvNV
	static inline void glNamedFramebufferSampleLocationsfvNV(GLuint framebuffer, GLuint start, GLsizei count, const GLfloat *v){
	    glad_debug_glNamedFramebufferSampleLocationsfvNV(framebuffer, start, count, v);
	}
#endif // glNamedFramebufferSampleLocationsfvNV

#ifdef glNamedFramebufferTexture
#undef glNamedFramebufferTexture
	static inline void glNamedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level){
	    glad_debug_glNamedFramebufferTexture(framebuffer, attachment, texture, level);
	}
#endif // glNamedFramebufferTexture

#ifdef glNamedFramebufferSamplePositionsfvAMD
#undef glNamedFramebufferSamplePositionsfvAMD
	static inline void glNamedFramebufferSamplePositionsfvAMD(GLuint framebuffer, GLuint numsamples, GLuint pixelindex, const GLfloat *values){
	    glad_debug_glNamedFramebufferSamplePositionsfvAMD(framebuffer, numsamples, pixelindex, values);
	}
#endif // glNamedFramebufferSamplePositionsfvAMD

#ifdef glNamedFramebufferTexture1DEXT
#undef glNamedFramebufferTexture1DEXT
	static inline void glNamedFramebufferTexture1DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level){
	    glad_debug_glNamedFramebufferTexture1DEXT(framebuffer, attachment, textarget, texture, level);
	}
#endif // glNamedFramebufferTexture1DEXT

#ifdef glNamedFramebufferTexture2DEXT
#undef glNamedFramebufferTexture2DEXT
	static inline void glNamedFramebufferTexture2DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level){
	    glad_debug_glNamedFramebufferTexture2DEXT(framebuffer, attachment, textarget, texture, level);
	}
#endif // glNamedFramebufferTexture2DEXT

#ifdef glNamedFramebufferTexture3DEXT
#undef glNamedFramebufferTexture3DEXT
	static inline void glNamedFramebufferTexture3DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset){
	    glad_debug_glNamedFramebufferTexture3DEXT(framebuffer, attachment, textarget, texture, level, zoffset);
	}
#endif // glNamedFramebufferTexture3DEXT

#ifdef glNamedFramebufferTextureEXT
#undef glNamedFramebufferTextureEXT
	static inline void glNamedFramebufferTextureEXT(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level){
	    glad_debug_glNamedFramebufferTextureEXT(framebuffer, attachment, texture, level);
	}
#endif // glNamedFramebufferTextureEXT

#ifdef glNamedFramebufferTextureFaceEXT
#undef glNamedFramebufferTextureFaceEXT
	static inline void glNamedFramebufferTextureFaceEXT(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face){
	    glad_debug_glNamedFramebufferTextureFaceEXT(framebuffer, attachment, texture, level, face);
	}
#endif // glNamedFramebufferTextureFaceEXT

#ifdef glNamedFramebufferTextureLayer
#undef glNamedFramebufferTextureLayer
	static inline void glNamedFramebufferTextureLayer(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer){
	    glad_debug_glNamedFramebufferTextureLayer(framebuffer, attachment, texture, level, layer);
	}
#endif // glNamedFramebufferTextureLayer

#ifdef glNamedFramebufferTextureLayerEXT
#undef glNamedFramebufferTextureLayerEXT
	static inline void glNamedFramebufferTextureLayerEXT(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer){
	    glad_debug_glNamedFramebufferTextureLayerEXT(framebuffer, attachment, texture, level, layer);
	}
#endif // glNamedFramebufferTextureLayerEXT

#ifdef glNamedFramebufferTextureMultiviewOVR
#undef glNamedFramebufferTextureMultiviewOVR
	static inline void glNamedFramebufferTextureMultiviewOVR(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint baseViewIndex, GLsizei numViews){
	    glad_debug_glNamedFramebufferTextureMultiviewOVR(framebuffer, attachment, texture, level, baseViewIndex, numViews);
	}
#endif // glNamedFramebufferTextureMultiviewOVR

#ifdef glNamedProgramLocalParameter4dEXT
#undef glNamedProgramLocalParameter4dEXT
	static inline void glNamedProgramLocalParameter4dEXT(GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glNamedProgramLocalParameter4dEXT(program, target, index, x, y, z, w);
	}
#endif // glNamedProgramLocalParameter4dEXT

#ifdef glNamedProgramLocalParameter4dvEXT
#undef glNamedProgramLocalParameter4dvEXT
	static inline void glNamedProgramLocalParameter4dvEXT(GLuint program, GLenum target, GLuint index, const GLdouble *params){
	    glad_debug_glNamedProgramLocalParameter4dvEXT(program, target, index, params);
	}
#endif // glNamedProgramLocalParameter4dvEXT

#ifdef glNamedProgramLocalParameter4fEXT
#undef glNamedProgramLocalParameter4fEXT
	static inline void glNamedProgramLocalParameter4fEXT(GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glNamedProgramLocalParameter4fEXT(program, target, index, x, y, z, w);
	}
#endif // glNamedProgramLocalParameter4fEXT

#ifdef glNamedProgramLocalParameter4fvEXT
#undef glNamedProgramLocalParameter4fvEXT
	static inline void glNamedProgramLocalParameter4fvEXT(GLuint program, GLenum target, GLuint index, const GLfloat *params){
	    glad_debug_glNamedProgramLocalParameter4fvEXT(program, target, index, params);
	}
#endif // glNamedProgramLocalParameter4fvEXT

#ifdef glNamedProgramLocalParameterI4iEXT
#undef glNamedProgramLocalParameterI4iEXT
	static inline void glNamedProgramLocalParameterI4iEXT(GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w){
	    glad_debug_glNamedProgramLocalParameterI4iEXT(program, target, index, x, y, z, w);
	}
#endif // glNamedProgramLocalParameterI4iEXT

#ifdef glNamedProgramLocalParameterI4ivEXT
#undef glNamedProgramLocalParameterI4ivEXT
	static inline void glNamedProgramLocalParameterI4ivEXT(GLuint program, GLenum target, GLuint index, const GLint *params){
	    glad_debug_glNamedProgramLocalParameterI4ivEXT(program, target, index, params);
	}
#endif // glNamedProgramLocalParameterI4ivEXT

#ifdef glNamedProgramLocalParameterI4uiEXT
#undef glNamedProgramLocalParameterI4uiEXT
	static inline void glNamedProgramLocalParameterI4uiEXT(GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w){
	    glad_debug_glNamedProgramLocalParameterI4uiEXT(program, target, index, x, y, z, w);
	}
#endif // glNamedProgramLocalParameterI4uiEXT

#ifdef glNamedProgramLocalParameterI4uivEXT
#undef glNamedProgramLocalParameterI4uivEXT
	static inline void glNamedProgramLocalParameterI4uivEXT(GLuint program, GLenum target, GLuint index, const GLuint *params){
	    glad_debug_glNamedProgramLocalParameterI4uivEXT(program, target, index, params);
	}
#endif // glNamedProgramLocalParameterI4uivEXT

#ifdef glNamedProgramLocalParameters4fvEXT
#undef glNamedProgramLocalParameters4fvEXT
	static inline void glNamedProgramLocalParameters4fvEXT(GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat *params){
	    glad_debug_glNamedProgramLocalParameters4fvEXT(program, target, index, count, params);
	}
#endif // glNamedProgramLocalParameters4fvEXT

#ifdef glNamedProgramLocalParametersI4ivEXT
#undef glNamedProgramLocalParametersI4ivEXT
	static inline void glNamedProgramLocalParametersI4ivEXT(GLuint program, GLenum target, GLuint index, GLsizei count, const GLint *params){
	    glad_debug_glNamedProgramLocalParametersI4ivEXT(program, target, index, count, params);
	}
#endif // glNamedProgramLocalParametersI4ivEXT

#ifdef glNamedProgramLocalParametersI4uivEXT
#undef glNamedProgramLocalParametersI4uivEXT
	static inline void glNamedProgramLocalParametersI4uivEXT(GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint *params){
	    glad_debug_glNamedProgramLocalParametersI4uivEXT(program, target, index, count, params);
	}
#endif // glNamedProgramLocalParametersI4uivEXT

#ifdef glNamedRenderbufferStorage
#undef glNamedRenderbufferStorage
	static inline void glNamedRenderbufferStorage(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glNamedRenderbufferStorage(renderbuffer, internalformat, width, height);
	}
#endif // glNamedRenderbufferStorage

#ifdef glNamedRenderbufferStorageEXT
#undef glNamedRenderbufferStorageEXT
	static inline void glNamedRenderbufferStorageEXT(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glNamedRenderbufferStorageEXT(renderbuffer, internalformat, width, height);
	}
#endif // glNamedRenderbufferStorageEXT

#ifdef glNamedRenderbufferStorageMultisample
#undef glNamedRenderbufferStorageMultisample
	static inline void glNamedRenderbufferStorageMultisample(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glNamedRenderbufferStorageMultisample(renderbuffer, samples, internalformat, width, height);
	}
#endif // glNamedRenderbufferStorageMultisample

#ifdef glNamedRenderbufferStorageMultisampleAdvancedAMD
#undef glNamedRenderbufferStorageMultisampleAdvancedAMD
	static inline void glNamedRenderbufferStorageMultisampleAdvancedAMD(GLuint renderbuffer, GLsizei samples, GLsizei storageSamples, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glNamedRenderbufferStorageMultisampleAdvancedAMD(renderbuffer, samples, storageSamples, internalformat, width, height);
	}
#endif // glNamedRenderbufferStorageMultisampleAdvancedAMD

#ifdef glNamedRenderbufferStorageMultisampleCoverageEXT
#undef glNamedRenderbufferStorageMultisampleCoverageEXT
	static inline void glNamedRenderbufferStorageMultisampleCoverageEXT(GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glNamedRenderbufferStorageMultisampleCoverageEXT(renderbuffer, coverageSamples, colorSamples, internalformat, width, height);
	}
#endif // glNamedRenderbufferStorageMultisampleCoverageEXT

#ifdef glNamedRenderbufferStorageMultisampleEXT
#undef glNamedRenderbufferStorageMultisampleEXT
	static inline void glNamedRenderbufferStorageMultisampleEXT(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glNamedRenderbufferStorageMultisampleEXT(renderbuffer, samples, internalformat, width, height);
	}
#endif // glNamedRenderbufferStorageMultisampleEXT

#ifdef glNamedStringARB
#undef glNamedStringARB
	static inline void glNamedStringARB(GLenum type, GLint namelen, const GLchar *name, GLint stringlen, const GLchar *string){
	    glad_debug_glNamedStringARB(type, namelen, name, stringlen, string);
	}
#endif // glNamedStringARB

#ifdef glNewList
#undef glNewList
	static inline void glNewList(GLuint list, GLenum mode){
	    glad_debug_glNewList(list, mode);
	}
#endif // glNewList

#ifdef glNormal3b
#undef glNormal3b
	static inline void glNormal3b(GLbyte nx, GLbyte ny, GLbyte nz){
	    glad_debug_glNormal3b(nx, ny, nz);
	}
#endif // glNormal3b

#ifdef glNormal3bv
#undef glNormal3bv
	static inline void glNormal3bv(const GLbyte *v){
	    glad_debug_glNormal3bv(v);
	}
#endif // glNormal3bv

#ifdef glNormal3d
#undef glNormal3d
	static inline void glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz){
	    glad_debug_glNormal3d(nx, ny, nz);
	}
#endif // glNormal3d

#ifdef glNormal3dv
#undef glNormal3dv
	static inline void glNormal3dv(const GLdouble *v){
	    glad_debug_glNormal3dv(v);
	}
#endif // glNormal3dv

#ifdef glNormal3f
#undef glNormal3f
	static inline void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz){
	    glad_debug_glNormal3f(nx, ny, nz);
	}
#endif // glNormal3f

#ifdef glNormal3fVertex3fSUN
#undef glNormal3fVertex3fSUN
	static inline void glNormal3fVertex3fSUN(GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glNormal3fVertex3fSUN(nx, ny, nz, x, y, z);
	}
#endif // glNormal3fVertex3fSUN

#ifdef glNormal3fVertex3fvSUN
#undef glNormal3fVertex3fvSUN
	static inline void glNormal3fVertex3fvSUN(const GLfloat *n, const GLfloat *v){
	    glad_debug_glNormal3fVertex3fvSUN(n, v);
	}
#endif // glNormal3fVertex3fvSUN

#ifdef glNormal3fv
#undef glNormal3fv
	static inline void glNormal3fv(const GLfloat *v){
	    glad_debug_glNormal3fv(v);
	}
#endif // glNormal3fv

#ifdef glNormal3hNV
#undef glNormal3hNV
	static inline void glNormal3hNV(GLhalfNV nx, GLhalfNV ny, GLhalfNV nz){
	    glad_debug_glNormal3hNV(nx, ny, nz);
	}
#endif // glNormal3hNV

#ifdef glNormal3hvNV
#undef glNormal3hvNV
	static inline void glNormal3hvNV(const GLhalfNV *v){
	    glad_debug_glNormal3hvNV(v);
	}
#endif // glNormal3hvNV

#ifdef glNormal3i
#undef glNormal3i
	static inline void glNormal3i(GLint nx, GLint ny, GLint nz){
	    glad_debug_glNormal3i(nx, ny, nz);
	}
#endif // glNormal3i

#ifdef glNormal3iv
#undef glNormal3iv
	static inline void glNormal3iv(const GLint *v){
	    glad_debug_glNormal3iv(v);
	}
#endif // glNormal3iv

#ifdef glNormal3s
#undef glNormal3s
	static inline void glNormal3s(GLshort nx, GLshort ny, GLshort nz){
	    glad_debug_glNormal3s(nx, ny, nz);
	}
#endif // glNormal3s

#ifdef glNormal3sv
#undef glNormal3sv
	static inline void glNormal3sv(const GLshort *v){
	    glad_debug_glNormal3sv(v);
	}
#endif // glNormal3sv

#ifdef glNormal3x
#undef glNormal3x
	static inline void glNormal3x(GLfixed nx, GLfixed ny, GLfixed nz){
	    glad_debug_glNormal3x(nx, ny, nz);
	}
#endif // glNormal3x

#ifdef glNormal3xOES
#undef glNormal3xOES
	static inline void glNormal3xOES(GLfixed nx, GLfixed ny, GLfixed nz){
	    glad_debug_glNormal3xOES(nx, ny, nz);
	}
#endif // glNormal3xOES

#ifdef glNormal3xvOES
#undef glNormal3xvOES
	static inline void glNormal3xvOES(const GLfixed *coords){
	    glad_debug_glNormal3xvOES(coords);
	}
#endif // glNormal3xvOES

#ifdef glNormalFormatNV
#undef glNormalFormatNV
	static inline void glNormalFormatNV(GLenum type, GLsizei stride){
	    glad_debug_glNormalFormatNV(type, stride);
	}
#endif // glNormalFormatNV

#ifdef glNormalP3ui
#undef glNormalP3ui
	static inline void glNormalP3ui(GLenum type, GLuint coords){
	    glad_debug_glNormalP3ui(type, coords);
	}
#endif // glNormalP3ui

#ifdef glNormalP3uiv
#undef glNormalP3uiv
	static inline void glNormalP3uiv(GLenum type, const GLuint *coords){
	    glad_debug_glNormalP3uiv(type, coords);
	}
#endif // glNormalP3uiv

#ifdef glNormalStream3bATI
#undef glNormalStream3bATI
	static inline void glNormalStream3bATI(GLenum stream, GLbyte nx, GLbyte ny, GLbyte nz){
	    glad_debug_glNormalStream3bATI(stream, nx, ny, nz);
	}
#endif // glNormalStream3bATI

#ifdef glNormalStream3bvATI
#undef glNormalStream3bvATI
	static inline void glNormalStream3bvATI(GLenum stream, const GLbyte *coords){
	    glad_debug_glNormalStream3bvATI(stream, coords);
	}
#endif // glNormalStream3bvATI

#ifdef glNormalStream3dATI
#undef glNormalStream3dATI
	static inline void glNormalStream3dATI(GLenum stream, GLdouble nx, GLdouble ny, GLdouble nz){
	    glad_debug_glNormalStream3dATI(stream, nx, ny, nz);
	}
#endif // glNormalStream3dATI

#ifdef glNormalStream3dvATI
#undef glNormalStream3dvATI
	static inline void glNormalStream3dvATI(GLenum stream, const GLdouble *coords){
	    glad_debug_glNormalStream3dvATI(stream, coords);
	}
#endif // glNormalStream3dvATI

#ifdef glNormalStream3fATI
#undef glNormalStream3fATI
	static inline void glNormalStream3fATI(GLenum stream, GLfloat nx, GLfloat ny, GLfloat nz){
	    glad_debug_glNormalStream3fATI(stream, nx, ny, nz);
	}
#endif // glNormalStream3fATI

#ifdef glNormalStream3fvATI
#undef glNormalStream3fvATI
	static inline void glNormalStream3fvATI(GLenum stream, const GLfloat *coords){
	    glad_debug_glNormalStream3fvATI(stream, coords);
	}
#endif // glNormalStream3fvATI

#ifdef glNormalStream3iATI
#undef glNormalStream3iATI
	static inline void glNormalStream3iATI(GLenum stream, GLint nx, GLint ny, GLint nz){
	    glad_debug_glNormalStream3iATI(stream, nx, ny, nz);
	}
#endif // glNormalStream3iATI

#ifdef glNormalStream3ivATI
#undef glNormalStream3ivATI
	static inline void glNormalStream3ivATI(GLenum stream, const GLint *coords){
	    glad_debug_glNormalStream3ivATI(stream, coords);
	}
#endif // glNormalStream3ivATI

#ifdef glNormalStream3sATI
#undef glNormalStream3sATI
	static inline void glNormalStream3sATI(GLenum stream, GLshort nx, GLshort ny, GLshort nz){
	    glad_debug_glNormalStream3sATI(stream, nx, ny, nz);
	}
#endif // glNormalStream3sATI

#ifdef glNormalStream3svATI
#undef glNormalStream3svATI
	static inline void glNormalStream3svATI(GLenum stream, const GLshort *coords){
	    glad_debug_glNormalStream3svATI(stream, coords);
	}
#endif // glNormalStream3svATI

#ifdef glObjectLabel
#undef glObjectLabel
	static inline void glObjectLabel(GLenum identifier, GLuint name, GLsizei length, const GLchar *label){
	    glad_debug_glObjectLabel(identifier, name, length, label);
	}
#endif // glObjectLabel

#ifdef glObjectLabelKHR
#undef glObjectLabelKHR
	static inline void glObjectLabelKHR(GLenum identifier, GLuint name, GLsizei length, const GLchar *label){
	    glad_debug_glObjectLabelKHR(identifier, name, length, label);
	}
#endif // glObjectLabelKHR

#ifdef glOrtho
#undef glOrtho
	static inline void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar){
	    glad_debug_glOrtho(left, right, bottom, top, zNear, zFar);
	}
#endif // glOrtho

#ifdef glOrthof
#undef glOrthof
	static inline void glOrthof(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f){
	    glad_debug_glOrthof(l, r, b, t, n, f);
	}
#endif // glOrthof

#ifdef glOrthofOES
#undef glOrthofOES
	static inline void glOrthofOES(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f){
	    glad_debug_glOrthofOES(l, r, b, t, n, f);
	}
#endif // glOrthofOES

#ifdef glOrthox
#undef glOrthox
	static inline void glOrthox(GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f){
	    glad_debug_glOrthox(l, r, b, t, n, f);
	}
#endif // glOrthox

#ifdef glOrthoxOES
#undef glOrthoxOES
	static inline void glOrthoxOES(GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f){
	    glad_debug_glOrthoxOES(l, r, b, t, n, f);
	}
#endif // glOrthoxOES

#ifdef glPNTrianglesfATI
#undef glPNTrianglesfATI
	static inline void glPNTrianglesfATI(GLenum pname, GLfloat param){
	    glad_debug_glPNTrianglesfATI(pname, param);
	}
#endif // glPNTrianglesfATI

#ifdef glPNTrianglesiATI
#undef glPNTrianglesiATI
	static inline void glPNTrianglesiATI(GLenum pname, GLint param){
	    glad_debug_glPNTrianglesiATI(pname, param);
	}
#endif // glPNTrianglesiATI

#ifdef glPassTexCoordATI
#undef glPassTexCoordATI
	static inline void glPassTexCoordATI(GLuint dst, GLuint coord, GLenum swizzle){
	    glad_debug_glPassTexCoordATI(dst, coord, swizzle);
	}
#endif // glPassTexCoordATI

#ifdef glPassThrough
#undef glPassThrough
	static inline void glPassThrough(GLfloat token){
	    glad_debug_glPassThrough(token);
	}
#endif // glPassThrough

#ifdef glPassThroughxOES
#undef glPassThroughxOES
	static inline void glPassThroughxOES(GLfixed token){
	    glad_debug_glPassThroughxOES(token);
	}
#endif // glPassThroughxOES

#ifdef glPatchParameterfv
#undef glPatchParameterfv
	static inline void glPatchParameterfv(GLenum pname, const GLfloat *values){
	    glad_debug_glPatchParameterfv(pname, values);
	}
#endif // glPatchParameterfv

#ifdef glPatchParameteri
#undef glPatchParameteri
	static inline void glPatchParameteri(GLenum pname, GLint value){
	    glad_debug_glPatchParameteri(pname, value);
	}
#endif // glPatchParameteri

#ifdef glPatchParameteriEXT
#undef glPatchParameteriEXT
	static inline void glPatchParameteriEXT(GLenum pname, GLint value){
	    glad_debug_glPatchParameteriEXT(pname, value);
	}
#endif // glPatchParameteriEXT

#ifdef glPatchParameteriOES
#undef glPatchParameteriOES
	static inline void glPatchParameteriOES(GLenum pname, GLint value){
	    glad_debug_glPatchParameteriOES(pname, value);
	}
#endif // glPatchParameteriOES

#ifdef glPathColorGenNV
#undef glPathColorGenNV
	static inline void glPathColorGenNV(GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat *coeffs){
	    glad_debug_glPathColorGenNV(color, genMode, colorFormat, coeffs);
	}
#endif // glPathColorGenNV

#ifdef glPathCoverDepthFuncNV
#undef glPathCoverDepthFuncNV
	static inline void glPathCoverDepthFuncNV(GLenum func){
	    glad_debug_glPathCoverDepthFuncNV(func);
	}
#endif // glPathCoverDepthFuncNV

#ifdef glPathDashArrayNV
#undef glPathDashArrayNV
	static inline void glPathDashArrayNV(GLuint path, GLsizei dashCount, const GLfloat *dashArray){
	    glad_debug_glPathDashArrayNV(path, dashCount, dashArray);
	}
#endif // glPathDashArrayNV

#ifdef glPathFogGenNV
#undef glPathFogGenNV
	static inline void glPathFogGenNV(GLenum genMode){
	    glad_debug_glPathFogGenNV(genMode);
	}
#endif // glPathFogGenNV

#ifdef glPathParameterfNV
#undef glPathParameterfNV
	static inline void glPathParameterfNV(GLuint path, GLenum pname, GLfloat value){
	    glad_debug_glPathParameterfNV(path, pname, value);
	}
#endif // glPathParameterfNV

#ifdef glPathParameterfvNV
#undef glPathParameterfvNV
	static inline void glPathParameterfvNV(GLuint path, GLenum pname, const GLfloat *value){
	    glad_debug_glPathParameterfvNV(path, pname, value);
	}
#endif // glPathParameterfvNV

#ifdef glPathParameteriNV
#undef glPathParameteriNV
	static inline void glPathParameteriNV(GLuint path, GLenum pname, GLint value){
	    glad_debug_glPathParameteriNV(path, pname, value);
	}
#endif // glPathParameteriNV

#ifdef glPathParameterivNV
#undef glPathParameterivNV
	static inline void glPathParameterivNV(GLuint path, GLenum pname, const GLint *value){
	    glad_debug_glPathParameterivNV(path, pname, value);
	}
#endif // glPathParameterivNV

#ifdef glPathStencilDepthOffsetNV
#undef glPathStencilDepthOffsetNV
	static inline void glPathStencilDepthOffsetNV(GLfloat factor, GLfloat units){
	    glad_debug_glPathStencilDepthOffsetNV(factor, units);
	}
#endif // glPathStencilDepthOffsetNV

#ifdef glPathStencilFuncNV
#undef glPathStencilFuncNV
	static inline void glPathStencilFuncNV(GLenum func, GLint ref, GLuint mask){
	    glad_debug_glPathStencilFuncNV(func, ref, mask);
	}
#endif // glPathStencilFuncNV

#ifdef glPathTexGenNV
#undef glPathTexGenNV
	static inline void glPathTexGenNV(GLenum texCoordSet, GLenum genMode, GLint components, const GLfloat *coeffs){
	    glad_debug_glPathTexGenNV(texCoordSet, genMode, components, coeffs);
	}
#endif // glPathTexGenNV

#ifdef glPauseTransformFeedback
#undef glPauseTransformFeedback
	static inline void glPauseTransformFeedback(){
	    glad_debug_glPauseTransformFeedback();
	}
#endif // glPauseTransformFeedback

#ifdef glPauseTransformFeedbackNV
#undef glPauseTransformFeedbackNV
	static inline void glPauseTransformFeedbackNV(){
	    glad_debug_glPauseTransformFeedbackNV();
	}
#endif // glPauseTransformFeedbackNV

#ifdef glPixelMapfv
#undef glPixelMapfv
	static inline void glPixelMapfv(GLenum map, GLsizei mapsize, const GLfloat *values){
	    glad_debug_glPixelMapfv(map, mapsize, values);
	}
#endif // glPixelMapfv

#ifdef glPixelMapuiv
#undef glPixelMapuiv
	static inline void glPixelMapuiv(GLenum map, GLsizei mapsize, const GLuint *values){
	    glad_debug_glPixelMapuiv(map, mapsize, values);
	}
#endif // glPixelMapuiv

#ifdef glPixelMapusv
#undef glPixelMapusv
	static inline void glPixelMapusv(GLenum map, GLsizei mapsize, const GLushort *values){
	    glad_debug_glPixelMapusv(map, mapsize, values);
	}
#endif // glPixelMapusv

#ifdef glPixelMapx
#undef glPixelMapx
	static inline void glPixelMapx(GLenum map, GLint size, const GLfixed *values){
	    glad_debug_glPixelMapx(map, size, values);
	}
#endif // glPixelMapx

#ifdef glPixelStoref
#undef glPixelStoref
	static inline void glPixelStoref(GLenum pname, GLfloat param){
	    glad_debug_glPixelStoref(pname, param);
	}
#endif // glPixelStoref

#ifdef glPixelStorei
#undef glPixelStorei
	static inline void glPixelStorei(GLenum pname, GLint param){
	    glad_debug_glPixelStorei(pname, param);
	}
#endif // glPixelStorei

#ifdef glPixelStorex
#undef glPixelStorex
	static inline void glPixelStorex(GLenum pname, GLfixed param){
	    glad_debug_glPixelStorex(pname, param);
	}
#endif // glPixelStorex

#ifdef glPixelTexGenParameterfSGIS
#undef glPixelTexGenParameterfSGIS
	static inline void glPixelTexGenParameterfSGIS(GLenum pname, GLfloat param){
	    glad_debug_glPixelTexGenParameterfSGIS(pname, param);
	}
#endif // glPixelTexGenParameterfSGIS

#ifdef glPixelTexGenParameterfvSGIS
#undef glPixelTexGenParameterfvSGIS
	static inline void glPixelTexGenParameterfvSGIS(GLenum pname, const GLfloat *params){
	    glad_debug_glPixelTexGenParameterfvSGIS(pname, params);
	}
#endif // glPixelTexGenParameterfvSGIS

#ifdef glPixelTexGenParameteriSGIS
#undef glPixelTexGenParameteriSGIS
	static inline void glPixelTexGenParameteriSGIS(GLenum pname, GLint param){
	    glad_debug_glPixelTexGenParameteriSGIS(pname, param);
	}
#endif // glPixelTexGenParameteriSGIS

#ifdef glPixelTexGenParameterivSGIS
#undef glPixelTexGenParameterivSGIS
	static inline void glPixelTexGenParameterivSGIS(GLenum pname, const GLint *params){
	    glad_debug_glPixelTexGenParameterivSGIS(pname, params);
	}
#endif // glPixelTexGenParameterivSGIS

#ifdef glPixelTexGenSGIX
#undef glPixelTexGenSGIX
	static inline void glPixelTexGenSGIX(GLenum mode){
	    glad_debug_glPixelTexGenSGIX(mode);
	}
#endif // glPixelTexGenSGIX

#ifdef glPixelTransferf
#undef glPixelTransferf
	static inline void glPixelTransferf(GLenum pname, GLfloat param){
	    glad_debug_glPixelTransferf(pname, param);
	}
#endif // glPixelTransferf

#ifdef glPixelTransferi
#undef glPixelTransferi
	static inline void glPixelTransferi(GLenum pname, GLint param){
	    glad_debug_glPixelTransferi(pname, param);
	}
#endif // glPixelTransferi

#ifdef glPixelTransferxOES
#undef glPixelTransferxOES
	static inline void glPixelTransferxOES(GLenum pname, GLfixed param){
	    glad_debug_glPixelTransferxOES(pname, param);
	}
#endif // glPixelTransferxOES

#ifdef glPixelTransformParameterfEXT
#undef glPixelTransformParameterfEXT
	static inline void glPixelTransformParameterfEXT(GLenum target, GLenum pname, GLfloat param){
	    glad_debug_glPixelTransformParameterfEXT(target, pname, param);
	}
#endif // glPixelTransformParameterfEXT

#ifdef glPixelTransformParameterfvEXT
#undef glPixelTransformParameterfvEXT
	static inline void glPixelTransformParameterfvEXT(GLenum target, GLenum pname, const GLfloat *params){
	    glad_debug_glPixelTransformParameterfvEXT(target, pname, params);
	}
#endif // glPixelTransformParameterfvEXT

#ifdef glPixelTransformParameteriEXT
#undef glPixelTransformParameteriEXT
	static inline void glPixelTransformParameteriEXT(GLenum target, GLenum pname, GLint param){
	    glad_debug_glPixelTransformParameteriEXT(target, pname, param);
	}
#endif // glPixelTransformParameteriEXT

#ifdef glPixelTransformParameterivEXT
#undef glPixelTransformParameterivEXT
	static inline void glPixelTransformParameterivEXT(GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glPixelTransformParameterivEXT(target, pname, params);
	}
#endif // glPixelTransformParameterivEXT

#ifdef glPixelZoom
#undef glPixelZoom
	static inline void glPixelZoom(GLfloat xfactor, GLfloat yfactor){
	    glad_debug_glPixelZoom(xfactor, yfactor);
	}
#endif // glPixelZoom

#ifdef glPixelZoomxOES
#undef glPixelZoomxOES
	static inline void glPixelZoomxOES(GLfixed xfactor, GLfixed yfactor){
	    glad_debug_glPixelZoomxOES(xfactor, yfactor);
	}
#endif // glPixelZoomxOES

#ifdef glPointParameterf
#undef glPointParameterf
	static inline void glPointParameterf(GLenum pname, GLfloat param){
	    glad_debug_glPointParameterf(pname, param);
	}
#endif // glPointParameterf

#ifdef glPointParameterfARB
#undef glPointParameterfARB
	static inline void glPointParameterfARB(GLenum pname, GLfloat param){
	    glad_debug_glPointParameterfARB(pname, param);
	}
#endif // glPointParameterfARB

#ifdef glPointParameterfEXT
#undef glPointParameterfEXT
	static inline void glPointParameterfEXT(GLenum pname, GLfloat param){
	    glad_debug_glPointParameterfEXT(pname, param);
	}
#endif // glPointParameterfEXT

#ifdef glPointParameterfSGIS
#undef glPointParameterfSGIS
	static inline void glPointParameterfSGIS(GLenum pname, GLfloat param){
	    glad_debug_glPointParameterfSGIS(pname, param);
	}
#endif // glPointParameterfSGIS

#ifdef glPointParameterfv
#undef glPointParameterfv
	static inline void glPointParameterfv(GLenum pname, const GLfloat *params){
	    glad_debug_glPointParameterfv(pname, params);
	}
#endif // glPointParameterfv

#ifdef glPointParameterfvARB
#undef glPointParameterfvARB
	static inline void glPointParameterfvARB(GLenum pname, const GLfloat *params){
	    glad_debug_glPointParameterfvARB(pname, params);
	}
#endif // glPointParameterfvARB

#ifdef glPointParameterfvEXT
#undef glPointParameterfvEXT
	static inline void glPointParameterfvEXT(GLenum pname, const GLfloat *params){
	    glad_debug_glPointParameterfvEXT(pname, params);
	}
#endif // glPointParameterfvEXT

#ifdef glPointParameterfvSGIS
#undef glPointParameterfvSGIS
	static inline void glPointParameterfvSGIS(GLenum pname, const GLfloat *params){
	    glad_debug_glPointParameterfvSGIS(pname, params);
	}
#endif // glPointParameterfvSGIS

#ifdef glPointParameteri
#undef glPointParameteri
	static inline void glPointParameteri(GLenum pname, GLint param){
	    glad_debug_glPointParameteri(pname, param);
	}
#endif // glPointParameteri

#ifdef glPointParameteriNV
#undef glPointParameteriNV
	static inline void glPointParameteriNV(GLenum pname, GLint param){
	    glad_debug_glPointParameteriNV(pname, param);
	}
#endif // glPointParameteriNV

#ifdef glPointParameteriv
#undef glPointParameteriv
	static inline void glPointParameteriv(GLenum pname, const GLint *params){
	    glad_debug_glPointParameteriv(pname, params);
	}
#endif // glPointParameteriv

#ifdef glPointParameterivNV
#undef glPointParameterivNV
	static inline void glPointParameterivNV(GLenum pname, const GLint *params){
	    glad_debug_glPointParameterivNV(pname, params);
	}
#endif // glPointParameterivNV

#ifdef glPointParameterx
#undef glPointParameterx
	static inline void glPointParameterx(GLenum pname, GLfixed param){
	    glad_debug_glPointParameterx(pname, param);
	}
#endif // glPointParameterx

#ifdef glPointParameterxOES
#undef glPointParameterxOES
	static inline void glPointParameterxOES(GLenum pname, GLfixed param){
	    glad_debug_glPointParameterxOES(pname, param);
	}
#endif // glPointParameterxOES

#ifdef glPointParameterxv
#undef glPointParameterxv
	static inline void glPointParameterxv(GLenum pname, const GLfixed *params){
	    glad_debug_glPointParameterxv(pname, params);
	}
#endif // glPointParameterxv

#ifdef glPointParameterxvOES
#undef glPointParameterxvOES
	static inline void glPointParameterxvOES(GLenum pname, const GLfixed *params){
	    glad_debug_glPointParameterxvOES(pname, params);
	}
#endif // glPointParameterxvOES

#ifdef glPointSize
#undef glPointSize
	static inline void glPointSize(GLfloat size){
	    glad_debug_glPointSize(size);
	}
#endif // glPointSize

#ifdef glPointSizex
#undef glPointSizex
	static inline void glPointSizex(GLfixed size){
	    glad_debug_glPointSizex(size);
	}
#endif // glPointSizex

#ifdef glPointSizexOES
#undef glPointSizexOES
	static inline void glPointSizexOES(GLfixed size){
	    glad_debug_glPointSizexOES(size);
	}
#endif // glPointSizexOES

#ifdef glPolygonMode
#undef glPolygonMode
	static inline void glPolygonMode(GLenum face, GLenum mode){
	    glad_debug_glPolygonMode(face, mode);
	}
#endif // glPolygonMode

#ifdef glPolygonModeNV
#undef glPolygonModeNV
	static inline void glPolygonModeNV(GLenum face, GLenum mode){
	    glad_debug_glPolygonModeNV(face, mode);
	}
#endif // glPolygonModeNV

#ifdef glPolygonOffset
#undef glPolygonOffset
	static inline void glPolygonOffset(GLfloat factor, GLfloat units){
	    glad_debug_glPolygonOffset(factor, units);
	}
#endif // glPolygonOffset

#ifdef glPolygonOffsetClamp
#undef glPolygonOffsetClamp
	static inline void glPolygonOffsetClamp(GLfloat factor, GLfloat units, GLfloat clamp){
	    glad_debug_glPolygonOffsetClamp(factor, units, clamp);
	}
#endif // glPolygonOffsetClamp

#ifdef glPolygonOffsetClampEXT
#undef glPolygonOffsetClampEXT
	static inline void glPolygonOffsetClampEXT(GLfloat factor, GLfloat units, GLfloat clamp){
	    glad_debug_glPolygonOffsetClampEXT(factor, units, clamp);
	}
#endif // glPolygonOffsetClampEXT

#ifdef glPolygonOffsetEXT
#undef glPolygonOffsetEXT
	static inline void glPolygonOffsetEXT(GLfloat factor, GLfloat bias){
	    glad_debug_glPolygonOffsetEXT(factor, bias);
	}
#endif // glPolygonOffsetEXT

#ifdef glPolygonOffsetx
#undef glPolygonOffsetx
	static inline void glPolygonOffsetx(GLfixed factor, GLfixed units){
	    glad_debug_glPolygonOffsetx(factor, units);
	}
#endif // glPolygonOffsetx

#ifdef glPolygonOffsetxOES
#undef glPolygonOffsetxOES
	static inline void glPolygonOffsetxOES(GLfixed factor, GLfixed units){
	    glad_debug_glPolygonOffsetxOES(factor, units);
	}
#endif // glPolygonOffsetxOES

#ifdef glPolygonStipple
#undef glPolygonStipple
	static inline void glPolygonStipple(const GLubyte *mask){
	    glad_debug_glPolygonStipple(mask);
	}
#endif // glPolygonStipple

#ifdef glPopAttrib
#undef glPopAttrib
	static inline void glPopAttrib(){
	    glad_debug_glPopAttrib();
	}
#endif // glPopAttrib

#ifdef glPopClientAttrib
#undef glPopClientAttrib
	static inline void glPopClientAttrib(){
	    glad_debug_glPopClientAttrib();
	}
#endif // glPopClientAttrib

#ifdef glPopDebugGroup
#undef glPopDebugGroup
	static inline void glPopDebugGroup(){
	    glad_debug_glPopDebugGroup();
	}
#endif // glPopDebugGroup

#ifdef glPopDebugGroupKHR
#undef glPopDebugGroupKHR
	static inline void glPopDebugGroupKHR(){
	    glad_debug_glPopDebugGroupKHR();
	}
#endif // glPopDebugGroupKHR

#ifdef glPopGroupMarkerEXT
#undef glPopGroupMarkerEXT
	static inline void glPopGroupMarkerEXT(){
	    glad_debug_glPopGroupMarkerEXT();
	}
#endif // glPopGroupMarkerEXT

#ifdef glPopMatrix
#undef glPopMatrix
	static inline void glPopMatrix(){
	    glad_debug_glPopMatrix();
	}
#endif // glPopMatrix

#ifdef glPopName
#undef glPopName
	static inline void glPopName(){
	    glad_debug_glPopName();
	}
#endif // glPopName

#ifdef glPresentFrameDualFillNV
#undef glPresentFrameDualFillNV
	static inline void glPresentFrameDualFillNV(GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3){
	    glad_debug_glPresentFrameDualFillNV(video_slot, minPresentTime, beginPresentTimeId, presentDurationId, type, target0, fill0, target1, fill1, target2, fill2, target3, fill3);
	}
#endif // glPresentFrameDualFillNV

#ifdef glPresentFrameKeyedNV
#undef glPresentFrameKeyedNV
	static inline void glPresentFrameKeyedNV(GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1){
	    glad_debug_glPresentFrameKeyedNV(video_slot, minPresentTime, beginPresentTimeId, presentDurationId, type, target0, fill0, key0, target1, fill1, key1);
	}
#endif // glPresentFrameKeyedNV

#ifdef glPrimitiveBoundingBox
#undef glPrimitiveBoundingBox
	static inline void glPrimitiveBoundingBox(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW){
	    glad_debug_glPrimitiveBoundingBox(minX, minY, minZ, minW, maxX, maxY, maxZ, maxW);
	}
#endif // glPrimitiveBoundingBox

#ifdef glPrimitiveBoundingBoxARB
#undef glPrimitiveBoundingBoxARB
	static inline void glPrimitiveBoundingBoxARB(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW){
	    glad_debug_glPrimitiveBoundingBoxARB(minX, minY, minZ, minW, maxX, maxY, maxZ, maxW);
	}
#endif // glPrimitiveBoundingBoxARB

#ifdef glPrimitiveBoundingBoxEXT
#undef glPrimitiveBoundingBoxEXT
	static inline void glPrimitiveBoundingBoxEXT(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW){
	    glad_debug_glPrimitiveBoundingBoxEXT(minX, minY, minZ, minW, maxX, maxY, maxZ, maxW);
	}
#endif // glPrimitiveBoundingBoxEXT

#ifdef glPrimitiveBoundingBoxOES
#undef glPrimitiveBoundingBoxOES
	static inline void glPrimitiveBoundingBoxOES(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW){
	    glad_debug_glPrimitiveBoundingBoxOES(minX, minY, minZ, minW, maxX, maxY, maxZ, maxW);
	}
#endif // glPrimitiveBoundingBoxOES

#ifdef glPrimitiveRestartIndex
#undef glPrimitiveRestartIndex
	static inline void glPrimitiveRestartIndex(GLuint index){
	    glad_debug_glPrimitiveRestartIndex(index);
	}
#endif // glPrimitiveRestartIndex

#ifdef glPrimitiveRestartIndexNV
#undef glPrimitiveRestartIndexNV
	static inline void glPrimitiveRestartIndexNV(GLuint index){
	    glad_debug_glPrimitiveRestartIndexNV(index);
	}
#endif // glPrimitiveRestartIndexNV

#ifdef glPrimitiveRestartNV
#undef glPrimitiveRestartNV
	static inline void glPrimitiveRestartNV(){
	    glad_debug_glPrimitiveRestartNV();
	}
#endif // glPrimitiveRestartNV

#ifdef glPrioritizeTextures
#undef glPrioritizeTextures
	static inline void glPrioritizeTextures(GLsizei n, const GLuint *textures, const GLfloat *priorities){
	    glad_debug_glPrioritizeTextures(n, textures, priorities);
	}
#endif // glPrioritizeTextures

#ifdef glPrioritizeTexturesEXT
#undef glPrioritizeTexturesEXT
	static inline void glPrioritizeTexturesEXT(GLsizei n, const GLuint *textures, const GLclampf *priorities){
	    glad_debug_glPrioritizeTexturesEXT(n, textures, priorities);
	}
#endif // glPrioritizeTexturesEXT

#ifdef glPrioritizeTexturesxOES
#undef glPrioritizeTexturesxOES
	static inline void glPrioritizeTexturesxOES(GLsizei n, const GLuint *textures, const GLfixed *priorities){
	    glad_debug_glPrioritizeTexturesxOES(n, textures, priorities);
	}
#endif // glPrioritizeTexturesxOES

#ifdef glProgramBufferParametersIivNV
#undef glProgramBufferParametersIivNV
	static inline void glProgramBufferParametersIivNV(GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLint *params){
	    glad_debug_glProgramBufferParametersIivNV(target, bindingIndex, wordIndex, count, params);
	}
#endif // glProgramBufferParametersIivNV

#ifdef glProgramBufferParametersIuivNV
#undef glProgramBufferParametersIuivNV
	static inline void glProgramBufferParametersIuivNV(GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLuint *params){
	    glad_debug_glProgramBufferParametersIuivNV(target, bindingIndex, wordIndex, count, params);
	}
#endif // glProgramBufferParametersIuivNV

#ifdef glProgramBufferParametersfvNV
#undef glProgramBufferParametersfvNV
	static inline void glProgramBufferParametersfvNV(GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLfloat *params){
	    glad_debug_glProgramBufferParametersfvNV(target, bindingIndex, wordIndex, count, params);
	}
#endif // glProgramBufferParametersfvNV

#ifdef glProgramEnvParameter4dARB
#undef glProgramEnvParameter4dARB
	static inline void glProgramEnvParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glProgramEnvParameter4dARB(target, index, x, y, z, w);
	}
#endif // glProgramEnvParameter4dARB

#ifdef glProgramEnvParameter4dvARB
#undef glProgramEnvParameter4dvARB
	static inline void glProgramEnvParameter4dvARB(GLenum target, GLuint index, const GLdouble *params){
	    glad_debug_glProgramEnvParameter4dvARB(target, index, params);
	}
#endif // glProgramEnvParameter4dvARB

#ifdef glProgramEnvParameter4fARB
#undef glProgramEnvParameter4fARB
	static inline void glProgramEnvParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glProgramEnvParameter4fARB(target, index, x, y, z, w);
	}
#endif // glProgramEnvParameter4fARB

#ifdef glProgramEnvParameter4fvARB
#undef glProgramEnvParameter4fvARB
	static inline void glProgramEnvParameter4fvARB(GLenum target, GLuint index, const GLfloat *params){
	    glad_debug_glProgramEnvParameter4fvARB(target, index, params);
	}
#endif // glProgramEnvParameter4fvARB

#ifdef glProgramEnvParameterI4iNV
#undef glProgramEnvParameterI4iNV
	static inline void glProgramEnvParameterI4iNV(GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w){
	    glad_debug_glProgramEnvParameterI4iNV(target, index, x, y, z, w);
	}
#endif // glProgramEnvParameterI4iNV

#ifdef glProgramEnvParameterI4ivNV
#undef glProgramEnvParameterI4ivNV
	static inline void glProgramEnvParameterI4ivNV(GLenum target, GLuint index, const GLint *params){
	    glad_debug_glProgramEnvParameterI4ivNV(target, index, params);
	}
#endif // glProgramEnvParameterI4ivNV

#ifdef glProgramEnvParameterI4uiNV
#undef glProgramEnvParameterI4uiNV
	static inline void glProgramEnvParameterI4uiNV(GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w){
	    glad_debug_glProgramEnvParameterI4uiNV(target, index, x, y, z, w);
	}
#endif // glProgramEnvParameterI4uiNV

#ifdef glProgramEnvParameterI4uivNV
#undef glProgramEnvParameterI4uivNV
	static inline void glProgramEnvParameterI4uivNV(GLenum target, GLuint index, const GLuint *params){
	    glad_debug_glProgramEnvParameterI4uivNV(target, index, params);
	}
#endif // glProgramEnvParameterI4uivNV

#ifdef glProgramEnvParameters4fvEXT
#undef glProgramEnvParameters4fvEXT
	static inline void glProgramEnvParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat *params){
	    glad_debug_glProgramEnvParameters4fvEXT(target, index, count, params);
	}
#endif // glProgramEnvParameters4fvEXT

#ifdef glProgramEnvParametersI4ivNV
#undef glProgramEnvParametersI4ivNV
	static inline void glProgramEnvParametersI4ivNV(GLenum target, GLuint index, GLsizei count, const GLint *params){
	    glad_debug_glProgramEnvParametersI4ivNV(target, index, count, params);
	}
#endif // glProgramEnvParametersI4ivNV

#ifdef glProgramEnvParametersI4uivNV
#undef glProgramEnvParametersI4uivNV
	static inline void glProgramEnvParametersI4uivNV(GLenum target, GLuint index, GLsizei count, const GLuint *params){
	    glad_debug_glProgramEnvParametersI4uivNV(target, index, count, params);
	}
#endif // glProgramEnvParametersI4uivNV

#ifdef glProgramLocalParameter4dARB
#undef glProgramLocalParameter4dARB
	static inline void glProgramLocalParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glProgramLocalParameter4dARB(target, index, x, y, z, w);
	}
#endif // glProgramLocalParameter4dARB

#ifdef glProgramLocalParameter4dvARB
#undef glProgramLocalParameter4dvARB
	static inline void glProgramLocalParameter4dvARB(GLenum target, GLuint index, const GLdouble *params){
	    glad_debug_glProgramLocalParameter4dvARB(target, index, params);
	}
#endif // glProgramLocalParameter4dvARB

#ifdef glProgramLocalParameter4fARB
#undef glProgramLocalParameter4fARB
	static inline void glProgramLocalParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glProgramLocalParameter4fARB(target, index, x, y, z, w);
	}
#endif // glProgramLocalParameter4fARB

#ifdef glProgramLocalParameter4fvARB
#undef glProgramLocalParameter4fvARB
	static inline void glProgramLocalParameter4fvARB(GLenum target, GLuint index, const GLfloat *params){
	    glad_debug_glProgramLocalParameter4fvARB(target, index, params);
	}
#endif // glProgramLocalParameter4fvARB

#ifdef glProgramLocalParameterI4iNV
#undef glProgramLocalParameterI4iNV
	static inline void glProgramLocalParameterI4iNV(GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w){
	    glad_debug_glProgramLocalParameterI4iNV(target, index, x, y, z, w);
	}
#endif // glProgramLocalParameterI4iNV

#ifdef glProgramLocalParameterI4ivNV
#undef glProgramLocalParameterI4ivNV
	static inline void glProgramLocalParameterI4ivNV(GLenum target, GLuint index, const GLint *params){
	    glad_debug_glProgramLocalParameterI4ivNV(target, index, params);
	}
#endif // glProgramLocalParameterI4ivNV

#ifdef glProgramLocalParameterI4uiNV
#undef glProgramLocalParameterI4uiNV
	static inline void glProgramLocalParameterI4uiNV(GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w){
	    glad_debug_glProgramLocalParameterI4uiNV(target, index, x, y, z, w);
	}
#endif // glProgramLocalParameterI4uiNV

#ifdef glProgramLocalParameterI4uivNV
#undef glProgramLocalParameterI4uivNV
	static inline void glProgramLocalParameterI4uivNV(GLenum target, GLuint index, const GLuint *params){
	    glad_debug_glProgramLocalParameterI4uivNV(target, index, params);
	}
#endif // glProgramLocalParameterI4uivNV

#ifdef glProgramLocalParameters4fvEXT
#undef glProgramLocalParameters4fvEXT
	static inline void glProgramLocalParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat *params){
	    glad_debug_glProgramLocalParameters4fvEXT(target, index, count, params);
	}
#endif // glProgramLocalParameters4fvEXT

#ifdef glProgramLocalParametersI4ivNV
#undef glProgramLocalParametersI4ivNV
	static inline void glProgramLocalParametersI4ivNV(GLenum target, GLuint index, GLsizei count, const GLint *params){
	    glad_debug_glProgramLocalParametersI4ivNV(target, index, count, params);
	}
#endif // glProgramLocalParametersI4ivNV

#ifdef glProgramLocalParametersI4uivNV
#undef glProgramLocalParametersI4uivNV
	static inline void glProgramLocalParametersI4uivNV(GLenum target, GLuint index, GLsizei count, const GLuint *params){
	    glad_debug_glProgramLocalParametersI4uivNV(target, index, count, params);
	}
#endif // glProgramLocalParametersI4uivNV

#ifdef glProgramNamedParameter4dNV
#undef glProgramNamedParameter4dNV
	static inline void glProgramNamedParameter4dNV(GLuint id, GLsizei len, const GLubyte *name, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glProgramNamedParameter4dNV(id, len, name, x, y, z, w);
	}
#endif // glProgramNamedParameter4dNV

#ifdef glProgramNamedParameter4dvNV
#undef glProgramNamedParameter4dvNV
	static inline void glProgramNamedParameter4dvNV(GLuint id, GLsizei len, const GLubyte *name, const GLdouble *v){
	    glad_debug_glProgramNamedParameter4dvNV(id, len, name, v);
	}
#endif // glProgramNamedParameter4dvNV

#ifdef glProgramNamedParameter4fNV
#undef glProgramNamedParameter4fNV
	static inline void glProgramNamedParameter4fNV(GLuint id, GLsizei len, const GLubyte *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glProgramNamedParameter4fNV(id, len, name, x, y, z, w);
	}
#endif // glProgramNamedParameter4fNV

#ifdef glProgramNamedParameter4fvNV
#undef glProgramNamedParameter4fvNV
	static inline void glProgramNamedParameter4fvNV(GLuint id, GLsizei len, const GLubyte *name, const GLfloat *v){
	    glad_debug_glProgramNamedParameter4fvNV(id, len, name, v);
	}
#endif // glProgramNamedParameter4fvNV

#ifdef glProgramParameter4dNV
#undef glProgramParameter4dNV
	static inline void glProgramParameter4dNV(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glProgramParameter4dNV(target, index, x, y, z, w);
	}
#endif // glProgramParameter4dNV

#ifdef glProgramParameter4dvNV
#undef glProgramParameter4dvNV
	static inline void glProgramParameter4dvNV(GLenum target, GLuint index, const GLdouble *v){
	    glad_debug_glProgramParameter4dvNV(target, index, v);
	}
#endif // glProgramParameter4dvNV

#ifdef glProgramParameter4fNV
#undef glProgramParameter4fNV
	static inline void glProgramParameter4fNV(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glProgramParameter4fNV(target, index, x, y, z, w);
	}
#endif // glProgramParameter4fNV

#ifdef glProgramParameter4fvNV
#undef glProgramParameter4fvNV
	static inline void glProgramParameter4fvNV(GLenum target, GLuint index, const GLfloat *v){
	    glad_debug_glProgramParameter4fvNV(target, index, v);
	}
#endif // glProgramParameter4fvNV

#ifdef glProgramParameteri
#undef glProgramParameteri
	static inline void glProgramParameteri(GLuint program, GLenum pname, GLint value){
	    glad_debug_glProgramParameteri(program, pname, value);
	}
#endif // glProgramParameteri

#ifdef glProgramParameteriARB
#undef glProgramParameteriARB
	static inline void glProgramParameteriARB(GLuint program, GLenum pname, GLint value){
	    glad_debug_glProgramParameteriARB(program, pname, value);
	}
#endif // glProgramParameteriARB

#ifdef glProgramParameteriEXT
#undef glProgramParameteriEXT
	static inline void glProgramParameteriEXT(GLuint program, GLenum pname, GLint value){
	    glad_debug_glProgramParameteriEXT(program, pname, value);
	}
#endif // glProgramParameteriEXT

#ifdef glProgramParameters4dvNV
#undef glProgramParameters4dvNV
	static inline void glProgramParameters4dvNV(GLenum target, GLuint index, GLsizei count, const GLdouble *v){
	    glad_debug_glProgramParameters4dvNV(target, index, count, v);
	}
#endif // glProgramParameters4dvNV

#ifdef glProgramParameters4fvNV
#undef glProgramParameters4fvNV
	static inline void glProgramParameters4fvNV(GLenum target, GLuint index, GLsizei count, const GLfloat *v){
	    glad_debug_glProgramParameters4fvNV(target, index, count, v);
	}
#endif // glProgramParameters4fvNV

#ifdef glProgramPathFragmentInputGenNV
#undef glProgramPathFragmentInputGenNV
	static inline void glProgramPathFragmentInputGenNV(GLuint program, GLint location, GLenum genMode, GLint components, const GLfloat *coeffs){
	    glad_debug_glProgramPathFragmentInputGenNV(program, location, genMode, components, coeffs);
	}
#endif // glProgramPathFragmentInputGenNV

#ifdef glProgramSubroutineParametersuivNV
#undef glProgramSubroutineParametersuivNV
	static inline void glProgramSubroutineParametersuivNV(GLenum target, GLsizei count, const GLuint *params){
	    glad_debug_glProgramSubroutineParametersuivNV(target, count, params);
	}
#endif // glProgramSubroutineParametersuivNV

#ifdef glProgramUniform1d
#undef glProgramUniform1d
	static inline void glProgramUniform1d(GLuint program, GLint location, GLdouble v0){
	    glad_debug_glProgramUniform1d(program, location, v0);
	}
#endif // glProgramUniform1d

#ifdef glProgramUniform1dEXT
#undef glProgramUniform1dEXT
	static inline void glProgramUniform1dEXT(GLuint program, GLint location, GLdouble x){
	    glad_debug_glProgramUniform1dEXT(program, location, x);
	}
#endif // glProgramUniform1dEXT

#ifdef glProgramUniform1dv
#undef glProgramUniform1dv
	static inline void glProgramUniform1dv(GLuint program, GLint location, GLsizei count, const GLdouble *value){
	    glad_debug_glProgramUniform1dv(program, location, count, value);
	}
#endif // glProgramUniform1dv

#ifdef glProgramUniform1dvEXT
#undef glProgramUniform1dvEXT
	static inline void glProgramUniform1dvEXT(GLuint program, GLint location, GLsizei count, const GLdouble *value){
	    glad_debug_glProgramUniform1dvEXT(program, location, count, value);
	}
#endif // glProgramUniform1dvEXT

#ifdef glProgramUniform1f
#undef glProgramUniform1f
	static inline void glProgramUniform1f(GLuint program, GLint location, GLfloat v0){
	    glad_debug_glProgramUniform1f(program, location, v0);
	}
#endif // glProgramUniform1f

#ifdef glProgramUniform1fEXT
#undef glProgramUniform1fEXT
	static inline void glProgramUniform1fEXT(GLuint program, GLint location, GLfloat v0){
	    glad_debug_glProgramUniform1fEXT(program, location, v0);
	}
#endif // glProgramUniform1fEXT

#ifdef glProgramUniform1fv
#undef glProgramUniform1fv
	static inline void glProgramUniform1fv(GLuint program, GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glProgramUniform1fv(program, location, count, value);
	}
#endif // glProgramUniform1fv

#ifdef glProgramUniform1fvEXT
#undef glProgramUniform1fvEXT
	static inline void glProgramUniform1fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glProgramUniform1fvEXT(program, location, count, value);
	}
#endif // glProgramUniform1fvEXT

#ifdef glProgramUniform1i
#undef glProgramUniform1i
	static inline void glProgramUniform1i(GLuint program, GLint location, GLint v0){
	    glad_debug_glProgramUniform1i(program, location, v0);
	}
#endif // glProgramUniform1i

#ifdef glProgramUniform1i64ARB
#undef glProgramUniform1i64ARB
	static inline void glProgramUniform1i64ARB(GLuint program, GLint location, GLint64 x){
	    glad_debug_glProgramUniform1i64ARB(program, location, x);
	}
#endif // glProgramUniform1i64ARB

#ifdef glProgramUniform1i64NV
#undef glProgramUniform1i64NV
	static inline void glProgramUniform1i64NV(GLuint program, GLint location, GLint64EXT x){
	    glad_debug_glProgramUniform1i64NV(program, location, x);
	}
#endif // glProgramUniform1i64NV

#ifdef glProgramUniform1i64vARB
#undef glProgramUniform1i64vARB
	static inline void glProgramUniform1i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 *value){
	    glad_debug_glProgramUniform1i64vARB(program, location, count, value);
	}
#endif // glProgramUniform1i64vARB

#ifdef glProgramUniform1i64vNV
#undef glProgramUniform1i64vNV
	static inline void glProgramUniform1i64vNV(GLuint program, GLint location, GLsizei count, const GLint64EXT *value){
	    glad_debug_glProgramUniform1i64vNV(program, location, count, value);
	}
#endif // glProgramUniform1i64vNV

#ifdef glProgramUniform1iEXT
#undef glProgramUniform1iEXT
	static inline void glProgramUniform1iEXT(GLuint program, GLint location, GLint v0){
	    glad_debug_glProgramUniform1iEXT(program, location, v0);
	}
#endif // glProgramUniform1iEXT

#ifdef glProgramUniform1iv
#undef glProgramUniform1iv
	static inline void glProgramUniform1iv(GLuint program, GLint location, GLsizei count, const GLint *value){
	    glad_debug_glProgramUniform1iv(program, location, count, value);
	}
#endif // glProgramUniform1iv

#ifdef glProgramUniform1ivEXT
#undef glProgramUniform1ivEXT
	static inline void glProgramUniform1ivEXT(GLuint program, GLint location, GLsizei count, const GLint *value){
	    glad_debug_glProgramUniform1ivEXT(program, location, count, value);
	}
#endif // glProgramUniform1ivEXT

#ifdef glProgramUniform1ui
#undef glProgramUniform1ui
	static inline void glProgramUniform1ui(GLuint program, GLint location, GLuint v0){
	    glad_debug_glProgramUniform1ui(program, location, v0);
	}
#endif // glProgramUniform1ui

#ifdef glProgramUniform1ui64ARB
#undef glProgramUniform1ui64ARB
	static inline void glProgramUniform1ui64ARB(GLuint program, GLint location, GLuint64 x){
	    glad_debug_glProgramUniform1ui64ARB(program, location, x);
	}
#endif // glProgramUniform1ui64ARB

#ifdef glProgramUniform1ui64NV
#undef glProgramUniform1ui64NV
	static inline void glProgramUniform1ui64NV(GLuint program, GLint location, GLuint64EXT x){
	    glad_debug_glProgramUniform1ui64NV(program, location, x);
	}
#endif // glProgramUniform1ui64NV

#ifdef glProgramUniform1ui64vARB
#undef glProgramUniform1ui64vARB
	static inline void glProgramUniform1ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *value){
	    glad_debug_glProgramUniform1ui64vARB(program, location, count, value);
	}
#endif // glProgramUniform1ui64vARB

#ifdef glProgramUniform1ui64vNV
#undef glProgramUniform1ui64vNV
	static inline void glProgramUniform1ui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64EXT *value){
	    glad_debug_glProgramUniform1ui64vNV(program, location, count, value);
	}
#endif // glProgramUniform1ui64vNV

#ifdef glProgramUniform1uiEXT
#undef glProgramUniform1uiEXT
	static inline void glProgramUniform1uiEXT(GLuint program, GLint location, GLuint v0){
	    glad_debug_glProgramUniform1uiEXT(program, location, v0);
	}
#endif // glProgramUniform1uiEXT

#ifdef glProgramUniform1uiv
#undef glProgramUniform1uiv
	static inline void glProgramUniform1uiv(GLuint program, GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glProgramUniform1uiv(program, location, count, value);
	}
#endif // glProgramUniform1uiv

#ifdef glProgramUniform1uivEXT
#undef glProgramUniform1uivEXT
	static inline void glProgramUniform1uivEXT(GLuint program, GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glProgramUniform1uivEXT(program, location, count, value);
	}
#endif // glProgramUniform1uivEXT

#ifdef glProgramUniform2d
#undef glProgramUniform2d
	static inline void glProgramUniform2d(GLuint program, GLint location, GLdouble v0, GLdouble v1){
	    glad_debug_glProgramUniform2d(program, location, v0, v1);
	}
#endif // glProgramUniform2d

#ifdef glProgramUniform2dEXT
#undef glProgramUniform2dEXT
	static inline void glProgramUniform2dEXT(GLuint program, GLint location, GLdouble x, GLdouble y){
	    glad_debug_glProgramUniform2dEXT(program, location, x, y);
	}
#endif // glProgramUniform2dEXT

#ifdef glProgramUniform2dv
#undef glProgramUniform2dv
	static inline void glProgramUniform2dv(GLuint program, GLint location, GLsizei count, const GLdouble *value){
	    glad_debug_glProgramUniform2dv(program, location, count, value);
	}
#endif // glProgramUniform2dv

#ifdef glProgramUniform2dvEXT
#undef glProgramUniform2dvEXT
	static inline void glProgramUniform2dvEXT(GLuint program, GLint location, GLsizei count, const GLdouble *value){
	    glad_debug_glProgramUniform2dvEXT(program, location, count, value);
	}
#endif // glProgramUniform2dvEXT

#ifdef glProgramUniform2f
#undef glProgramUniform2f
	static inline void glProgramUniform2f(GLuint program, GLint location, GLfloat v0, GLfloat v1){
	    glad_debug_glProgramUniform2f(program, location, v0, v1);
	}
#endif // glProgramUniform2f

#ifdef glProgramUniform2fEXT
#undef glProgramUniform2fEXT
	static inline void glProgramUniform2fEXT(GLuint program, GLint location, GLfloat v0, GLfloat v1){
	    glad_debug_glProgramUniform2fEXT(program, location, v0, v1);
	}
#endif // glProgramUniform2fEXT

#ifdef glProgramUniform2fv
#undef glProgramUniform2fv
	static inline void glProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glProgramUniform2fv(program, location, count, value);
	}
#endif // glProgramUniform2fv

#ifdef glProgramUniform2fvEXT
#undef glProgramUniform2fvEXT
	static inline void glProgramUniform2fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glProgramUniform2fvEXT(program, location, count, value);
	}
#endif // glProgramUniform2fvEXT

#ifdef glProgramUniform2i
#undef glProgramUniform2i
	static inline void glProgramUniform2i(GLuint program, GLint location, GLint v0, GLint v1){
	    glad_debug_glProgramUniform2i(program, location, v0, v1);
	}
#endif // glProgramUniform2i

#ifdef glProgramUniform2i64ARB
#undef glProgramUniform2i64ARB
	static inline void glProgramUniform2i64ARB(GLuint program, GLint location, GLint64 x, GLint64 y){
	    glad_debug_glProgramUniform2i64ARB(program, location, x, y);
	}
#endif // glProgramUniform2i64ARB

#ifdef glProgramUniform2i64NV
#undef glProgramUniform2i64NV
	static inline void glProgramUniform2i64NV(GLuint program, GLint location, GLint64EXT x, GLint64EXT y){
	    glad_debug_glProgramUniform2i64NV(program, location, x, y);
	}
#endif // glProgramUniform2i64NV

#ifdef glProgramUniform2i64vARB
#undef glProgramUniform2i64vARB
	static inline void glProgramUniform2i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 *value){
	    glad_debug_glProgramUniform2i64vARB(program, location, count, value);
	}
#endif // glProgramUniform2i64vARB

#ifdef glProgramUniform2i64vNV
#undef glProgramUniform2i64vNV
	static inline void glProgramUniform2i64vNV(GLuint program, GLint location, GLsizei count, const GLint64EXT *value){
	    glad_debug_glProgramUniform2i64vNV(program, location, count, value);
	}
#endif // glProgramUniform2i64vNV

#ifdef glProgramUniform2iEXT
#undef glProgramUniform2iEXT
	static inline void glProgramUniform2iEXT(GLuint program, GLint location, GLint v0, GLint v1){
	    glad_debug_glProgramUniform2iEXT(program, location, v0, v1);
	}
#endif // glProgramUniform2iEXT

#ifdef glProgramUniform2iv
#undef glProgramUniform2iv
	static inline void glProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint *value){
	    glad_debug_glProgramUniform2iv(program, location, count, value);
	}
#endif // glProgramUniform2iv

#ifdef glProgramUniform2ivEXT
#undef glProgramUniform2ivEXT
	static inline void glProgramUniform2ivEXT(GLuint program, GLint location, GLsizei count, const GLint *value){
	    glad_debug_glProgramUniform2ivEXT(program, location, count, value);
	}
#endif // glProgramUniform2ivEXT

#ifdef glProgramUniform2ui
#undef glProgramUniform2ui
	static inline void glProgramUniform2ui(GLuint program, GLint location, GLuint v0, GLuint v1){
	    glad_debug_glProgramUniform2ui(program, location, v0, v1);
	}
#endif // glProgramUniform2ui

#ifdef glProgramUniform2ui64ARB
#undef glProgramUniform2ui64ARB
	static inline void glProgramUniform2ui64ARB(GLuint program, GLint location, GLuint64 x, GLuint64 y){
	    glad_debug_glProgramUniform2ui64ARB(program, location, x, y);
	}
#endif // glProgramUniform2ui64ARB

#ifdef glProgramUniform2ui64NV
#undef glProgramUniform2ui64NV
	static inline void glProgramUniform2ui64NV(GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y){
	    glad_debug_glProgramUniform2ui64NV(program, location, x, y);
	}
#endif // glProgramUniform2ui64NV

#ifdef glProgramUniform2ui64vARB
#undef glProgramUniform2ui64vARB
	static inline void glProgramUniform2ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *value){
	    glad_debug_glProgramUniform2ui64vARB(program, location, count, value);
	}
#endif // glProgramUniform2ui64vARB

#ifdef glProgramUniform2ui64vNV
#undef glProgramUniform2ui64vNV
	static inline void glProgramUniform2ui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64EXT *value){
	    glad_debug_glProgramUniform2ui64vNV(program, location, count, value);
	}
#endif // glProgramUniform2ui64vNV

#ifdef glProgramUniform2uiEXT
#undef glProgramUniform2uiEXT
	static inline void glProgramUniform2uiEXT(GLuint program, GLint location, GLuint v0, GLuint v1){
	    glad_debug_glProgramUniform2uiEXT(program, location, v0, v1);
	}
#endif // glProgramUniform2uiEXT

#ifdef glProgramUniform2uiv
#undef glProgramUniform2uiv
	static inline void glProgramUniform2uiv(GLuint program, GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glProgramUniform2uiv(program, location, count, value);
	}
#endif // glProgramUniform2uiv

#ifdef glProgramUniform2uivEXT
#undef glProgramUniform2uivEXT
	static inline void glProgramUniform2uivEXT(GLuint program, GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glProgramUniform2uivEXT(program, location, count, value);
	}
#endif // glProgramUniform2uivEXT

#ifdef glProgramUniform3d
#undef glProgramUniform3d
	static inline void glProgramUniform3d(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2){
	    glad_debug_glProgramUniform3d(program, location, v0, v1, v2);
	}
#endif // glProgramUniform3d

#ifdef glProgramUniform3dEXT
#undef glProgramUniform3dEXT
	static inline void glProgramUniform3dEXT(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glProgramUniform3dEXT(program, location, x, y, z);
	}
#endif // glProgramUniform3dEXT

#ifdef glProgramUniform3dv
#undef glProgramUniform3dv
	static inline void glProgramUniform3dv(GLuint program, GLint location, GLsizei count, const GLdouble *value){
	    glad_debug_glProgramUniform3dv(program, location, count, value);
	}
#endif // glProgramUniform3dv

#ifdef glProgramUniform3dvEXT
#undef glProgramUniform3dvEXT
	static inline void glProgramUniform3dvEXT(GLuint program, GLint location, GLsizei count, const GLdouble *value){
	    glad_debug_glProgramUniform3dvEXT(program, location, count, value);
	}
#endif // glProgramUniform3dvEXT

#ifdef glProgramUniform3f
#undef glProgramUniform3f
	static inline void glProgramUniform3f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2){
	    glad_debug_glProgramUniform3f(program, location, v0, v1, v2);
	}
#endif // glProgramUniform3f

#ifdef glProgramUniform3fEXT
#undef glProgramUniform3fEXT
	static inline void glProgramUniform3fEXT(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2){
	    glad_debug_glProgramUniform3fEXT(program, location, v0, v1, v2);
	}
#endif // glProgramUniform3fEXT

#ifdef glProgramUniform3fv
#undef glProgramUniform3fv
	static inline void glProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glProgramUniform3fv(program, location, count, value);
	}
#endif // glProgramUniform3fv

#ifdef glProgramUniform3fvEXT
#undef glProgramUniform3fvEXT
	static inline void glProgramUniform3fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glProgramUniform3fvEXT(program, location, count, value);
	}
#endif // glProgramUniform3fvEXT

#ifdef glProgramUniform3i
#undef glProgramUniform3i
	static inline void glProgramUniform3i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2){
	    glad_debug_glProgramUniform3i(program, location, v0, v1, v2);
	}
#endif // glProgramUniform3i

#ifdef glProgramUniform3i64ARB
#undef glProgramUniform3i64ARB
	static inline void glProgramUniform3i64ARB(GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z){
	    glad_debug_glProgramUniform3i64ARB(program, location, x, y, z);
	}
#endif // glProgramUniform3i64ARB

#ifdef glProgramUniform3i64NV
#undef glProgramUniform3i64NV
	static inline void glProgramUniform3i64NV(GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z){
	    glad_debug_glProgramUniform3i64NV(program, location, x, y, z);
	}
#endif // glProgramUniform3i64NV

#ifdef glProgramUniform3i64vARB
#undef glProgramUniform3i64vARB
	static inline void glProgramUniform3i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 *value){
	    glad_debug_glProgramUniform3i64vARB(program, location, count, value);
	}
#endif // glProgramUniform3i64vARB

#ifdef glProgramUniform3i64vNV
#undef glProgramUniform3i64vNV
	static inline void glProgramUniform3i64vNV(GLuint program, GLint location, GLsizei count, const GLint64EXT *value){
	    glad_debug_glProgramUniform3i64vNV(program, location, count, value);
	}
#endif // glProgramUniform3i64vNV

#ifdef glProgramUniform3iEXT
#undef glProgramUniform3iEXT
	static inline void glProgramUniform3iEXT(GLuint program, GLint location, GLint v0, GLint v1, GLint v2){
	    glad_debug_glProgramUniform3iEXT(program, location, v0, v1, v2);
	}
#endif // glProgramUniform3iEXT

#ifdef glProgramUniform3iv
#undef glProgramUniform3iv
	static inline void glProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint *value){
	    glad_debug_glProgramUniform3iv(program, location, count, value);
	}
#endif // glProgramUniform3iv

#ifdef glProgramUniform3ivEXT
#undef glProgramUniform3ivEXT
	static inline void glProgramUniform3ivEXT(GLuint program, GLint location, GLsizei count, const GLint *value){
	    glad_debug_glProgramUniform3ivEXT(program, location, count, value);
	}
#endif // glProgramUniform3ivEXT

#ifdef glProgramUniform3ui
#undef glProgramUniform3ui
	static inline void glProgramUniform3ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2){
	    glad_debug_glProgramUniform3ui(program, location, v0, v1, v2);
	}
#endif // glProgramUniform3ui

#ifdef glProgramUniform3ui64ARB
#undef glProgramUniform3ui64ARB
	static inline void glProgramUniform3ui64ARB(GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z){
	    glad_debug_glProgramUniform3ui64ARB(program, location, x, y, z);
	}
#endif // glProgramUniform3ui64ARB

#ifdef glProgramUniform3ui64NV
#undef glProgramUniform3ui64NV
	static inline void glProgramUniform3ui64NV(GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z){
	    glad_debug_glProgramUniform3ui64NV(program, location, x, y, z);
	}
#endif // glProgramUniform3ui64NV

#ifdef glProgramUniform3ui64vARB
#undef glProgramUniform3ui64vARB
	static inline void glProgramUniform3ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *value){
	    glad_debug_glProgramUniform3ui64vARB(program, location, count, value);
	}
#endif // glProgramUniform3ui64vARB

#ifdef glProgramUniform3ui64vNV
#undef glProgramUniform3ui64vNV
	static inline void glProgramUniform3ui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64EXT *value){
	    glad_debug_glProgramUniform3ui64vNV(program, location, count, value);
	}
#endif // glProgramUniform3ui64vNV

#ifdef glProgramUniform3uiEXT
#undef glProgramUniform3uiEXT
	static inline void glProgramUniform3uiEXT(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2){
	    glad_debug_glProgramUniform3uiEXT(program, location, v0, v1, v2);
	}
#endif // glProgramUniform3uiEXT

#ifdef glProgramUniform3uiv
#undef glProgramUniform3uiv
	static inline void glProgramUniform3uiv(GLuint program, GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glProgramUniform3uiv(program, location, count, value);
	}
#endif // glProgramUniform3uiv

#ifdef glProgramUniform3uivEXT
#undef glProgramUniform3uivEXT
	static inline void glProgramUniform3uivEXT(GLuint program, GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glProgramUniform3uivEXT(program, location, count, value);
	}
#endif // glProgramUniform3uivEXT

#ifdef glProgramUniform4d
#undef glProgramUniform4d
	static inline void glProgramUniform4d(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3){
	    glad_debug_glProgramUniform4d(program, location, v0, v1, v2, v3);
	}
#endif // glProgramUniform4d

#ifdef glProgramUniform4dEXT
#undef glProgramUniform4dEXT
	static inline void glProgramUniform4dEXT(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glProgramUniform4dEXT(program, location, x, y, z, w);
	}
#endif // glProgramUniform4dEXT

#ifdef glProgramUniform4dv
#undef glProgramUniform4dv
	static inline void glProgramUniform4dv(GLuint program, GLint location, GLsizei count, const GLdouble *value){
	    glad_debug_glProgramUniform4dv(program, location, count, value);
	}
#endif // glProgramUniform4dv

#ifdef glProgramUniform4dvEXT
#undef glProgramUniform4dvEXT
	static inline void glProgramUniform4dvEXT(GLuint program, GLint location, GLsizei count, const GLdouble *value){
	    glad_debug_glProgramUniform4dvEXT(program, location, count, value);
	}
#endif // glProgramUniform4dvEXT

#ifdef glProgramUniform4f
#undef glProgramUniform4f
	static inline void glProgramUniform4f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3){
	    glad_debug_glProgramUniform4f(program, location, v0, v1, v2, v3);
	}
#endif // glProgramUniform4f

#ifdef glProgramUniform4fEXT
#undef glProgramUniform4fEXT
	static inline void glProgramUniform4fEXT(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3){
	    glad_debug_glProgramUniform4fEXT(program, location, v0, v1, v2, v3);
	}
#endif // glProgramUniform4fEXT

#ifdef glProgramUniform4fv
#undef glProgramUniform4fv
	static inline void glProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glProgramUniform4fv(program, location, count, value);
	}
#endif // glProgramUniform4fv

#ifdef glProgramUniform4fvEXT
#undef glProgramUniform4fvEXT
	static inline void glProgramUniform4fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glProgramUniform4fvEXT(program, location, count, value);
	}
#endif // glProgramUniform4fvEXT

#ifdef glProgramUniform4i
#undef glProgramUniform4i
	static inline void glProgramUniform4i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3){
	    glad_debug_glProgramUniform4i(program, location, v0, v1, v2, v3);
	}
#endif // glProgramUniform4i

#ifdef glProgramUniform4i64ARB
#undef glProgramUniform4i64ARB
	static inline void glProgramUniform4i64ARB(GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w){
	    glad_debug_glProgramUniform4i64ARB(program, location, x, y, z, w);
	}
#endif // glProgramUniform4i64ARB

#ifdef glProgramUniform4i64NV
#undef glProgramUniform4i64NV
	static inline void glProgramUniform4i64NV(GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w){
	    glad_debug_glProgramUniform4i64NV(program, location, x, y, z, w);
	}
#endif // glProgramUniform4i64NV

#ifdef glProgramUniform4i64vARB
#undef glProgramUniform4i64vARB
	static inline void glProgramUniform4i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 *value){
	    glad_debug_glProgramUniform4i64vARB(program, location, count, value);
	}
#endif // glProgramUniform4i64vARB

#ifdef glProgramUniform4i64vNV
#undef glProgramUniform4i64vNV
	static inline void glProgramUniform4i64vNV(GLuint program, GLint location, GLsizei count, const GLint64EXT *value){
	    glad_debug_glProgramUniform4i64vNV(program, location, count, value);
	}
#endif // glProgramUniform4i64vNV

#ifdef glProgramUniform4iEXT
#undef glProgramUniform4iEXT
	static inline void glProgramUniform4iEXT(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3){
	    glad_debug_glProgramUniform4iEXT(program, location, v0, v1, v2, v3);
	}
#endif // glProgramUniform4iEXT

#ifdef glProgramUniform4iv
#undef glProgramUniform4iv
	static inline void glProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint *value){
	    glad_debug_glProgramUniform4iv(program, location, count, value);
	}
#endif // glProgramUniform4iv

#ifdef glProgramUniform4ivEXT
#undef glProgramUniform4ivEXT
	static inline void glProgramUniform4ivEXT(GLuint program, GLint location, GLsizei count, const GLint *value){
	    glad_debug_glProgramUniform4ivEXT(program, location, count, value);
	}
#endif // glProgramUniform4ivEXT

#ifdef glProgramUniform4ui
#undef glProgramUniform4ui
	static inline void glProgramUniform4ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3){
	    glad_debug_glProgramUniform4ui(program, location, v0, v1, v2, v3);
	}
#endif // glProgramUniform4ui

#ifdef glProgramUniform4ui64ARB
#undef glProgramUniform4ui64ARB
	static inline void glProgramUniform4ui64ARB(GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w){
	    glad_debug_glProgramUniform4ui64ARB(program, location, x, y, z, w);
	}
#endif // glProgramUniform4ui64ARB

#ifdef glProgramUniform4ui64NV
#undef glProgramUniform4ui64NV
	static inline void glProgramUniform4ui64NV(GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w){
	    glad_debug_glProgramUniform4ui64NV(program, location, x, y, z, w);
	}
#endif // glProgramUniform4ui64NV

#ifdef glProgramUniform4ui64vARB
#undef glProgramUniform4ui64vARB
	static inline void glProgramUniform4ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *value){
	    glad_debug_glProgramUniform4ui64vARB(program, location, count, value);
	}
#endif // glProgramUniform4ui64vARB

#ifdef glProgramUniform4ui64vNV
#undef glProgramUniform4ui64vNV
	static inline void glProgramUniform4ui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64EXT *value){
	    glad_debug_glProgramUniform4ui64vNV(program, location, count, value);
	}
#endif // glProgramUniform4ui64vNV

#ifdef glProgramUniform4uiEXT
#undef glProgramUniform4uiEXT
	static inline void glProgramUniform4uiEXT(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3){
	    glad_debug_glProgramUniform4uiEXT(program, location, v0, v1, v2, v3);
	}
#endif // glProgramUniform4uiEXT

#ifdef glProgramUniform4uiv
#undef glProgramUniform4uiv
	static inline void glProgramUniform4uiv(GLuint program, GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glProgramUniform4uiv(program, location, count, value);
	}
#endif // glProgramUniform4uiv

#ifdef glProgramUniform4uivEXT
#undef glProgramUniform4uivEXT
	static inline void glProgramUniform4uivEXT(GLuint program, GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glProgramUniform4uivEXT(program, location, count, value);
	}
#endif // glProgramUniform4uivEXT

#ifdef glProgramUniformHandleui64ARB
#undef glProgramUniformHandleui64ARB
	static inline void glProgramUniformHandleui64ARB(GLuint program, GLint location, GLuint64 value){
	    glad_debug_glProgramUniformHandleui64ARB(program, location, value);
	}
#endif // glProgramUniformHandleui64ARB

#ifdef glProgramUniformHandleui64IMG
#undef glProgramUniformHandleui64IMG
	static inline void glProgramUniformHandleui64IMG(GLuint program, GLint location, GLuint64 value){
	    glad_debug_glProgramUniformHandleui64IMG(program, location, value);
	}
#endif // glProgramUniformHandleui64IMG

#ifdef glProgramUniformHandleui64NV
#undef glProgramUniformHandleui64NV
	static inline void glProgramUniformHandleui64NV(GLuint program, GLint location, GLuint64 value){
	    glad_debug_glProgramUniformHandleui64NV(program, location, value);
	}
#endif // glProgramUniformHandleui64NV

#ifdef glProgramUniformHandleui64vARB
#undef glProgramUniformHandleui64vARB
	static inline void glProgramUniformHandleui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *values){
	    glad_debug_glProgramUniformHandleui64vARB(program, location, count, values);
	}
#endif // glProgramUniformHandleui64vARB

#ifdef glProgramUniformHandleui64vIMG
#undef glProgramUniformHandleui64vIMG
	static inline void glProgramUniformHandleui64vIMG(GLuint program, GLint location, GLsizei count, const GLuint64 *values){
	    glad_debug_glProgramUniformHandleui64vIMG(program, location, count, values);
	}
#endif // glProgramUniformHandleui64vIMG

#ifdef glProgramUniformHandleui64vNV
#undef glProgramUniformHandleui64vNV
	static inline void glProgramUniformHandleui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64 *values){
	    glad_debug_glProgramUniformHandleui64vNV(program, location, count, values);
	}
#endif // glProgramUniformHandleui64vNV

#ifdef glProgramUniformMatrix2dv
#undef glProgramUniformMatrix2dv
	static inline void glProgramUniformMatrix2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix2dv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix2dv

#ifdef glProgramUniformMatrix2dvEXT
#undef glProgramUniformMatrix2dvEXT
	static inline void glProgramUniformMatrix2dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix2dvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix2dvEXT

#ifdef glProgramUniformMatrix2fv
#undef glProgramUniformMatrix2fv
	static inline void glProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix2fv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix2fv

#ifdef glProgramUniformMatrix2fvEXT
#undef glProgramUniformMatrix2fvEXT
	static inline void glProgramUniformMatrix2fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix2fvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix2fvEXT

#ifdef glProgramUniformMatrix2x3dv
#undef glProgramUniformMatrix2x3dv
	static inline void glProgramUniformMatrix2x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix2x3dv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix2x3dv

#ifdef glProgramUniformMatrix2x3dvEXT
#undef glProgramUniformMatrix2x3dvEXT
	static inline void glProgramUniformMatrix2x3dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix2x3dvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix2x3dvEXT

#ifdef glProgramUniformMatrix2x3fv
#undef glProgramUniformMatrix2x3fv
	static inline void glProgramUniformMatrix2x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix2x3fv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix2x3fv

#ifdef glProgramUniformMatrix2x3fvEXT
#undef glProgramUniformMatrix2x3fvEXT
	static inline void glProgramUniformMatrix2x3fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix2x3fvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix2x3fvEXT

#ifdef glProgramUniformMatrix2x4dv
#undef glProgramUniformMatrix2x4dv
	static inline void glProgramUniformMatrix2x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix2x4dv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix2x4dv

#ifdef glProgramUniformMatrix2x4dvEXT
#undef glProgramUniformMatrix2x4dvEXT
	static inline void glProgramUniformMatrix2x4dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix2x4dvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix2x4dvEXT

#ifdef glProgramUniformMatrix2x4fv
#undef glProgramUniformMatrix2x4fv
	static inline void glProgramUniformMatrix2x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix2x4fv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix2x4fv

#ifdef glProgramUniformMatrix2x4fvEXT
#undef glProgramUniformMatrix2x4fvEXT
	static inline void glProgramUniformMatrix2x4fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix2x4fvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix2x4fvEXT

#ifdef glProgramUniformMatrix3dv
#undef glProgramUniformMatrix3dv
	static inline void glProgramUniformMatrix3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix3dv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix3dv

#ifdef glProgramUniformMatrix3dvEXT
#undef glProgramUniformMatrix3dvEXT
	static inline void glProgramUniformMatrix3dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix3dvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix3dvEXT

#ifdef glProgramUniformMatrix3fv
#undef glProgramUniformMatrix3fv
	static inline void glProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix3fv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix3fv

#ifdef glProgramUniformMatrix3fvEXT
#undef glProgramUniformMatrix3fvEXT
	static inline void glProgramUniformMatrix3fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix3fvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix3fvEXT

#ifdef glProgramUniformMatrix3x2dv
#undef glProgramUniformMatrix3x2dv
	static inline void glProgramUniformMatrix3x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix3x2dv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix3x2dv

#ifdef glProgramUniformMatrix3x2dvEXT
#undef glProgramUniformMatrix3x2dvEXT
	static inline void glProgramUniformMatrix3x2dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix3x2dvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix3x2dvEXT

#ifdef glProgramUniformMatrix3x2fv
#undef glProgramUniformMatrix3x2fv
	static inline void glProgramUniformMatrix3x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix3x2fv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix3x2fv

#ifdef glProgramUniformMatrix3x2fvEXT
#undef glProgramUniformMatrix3x2fvEXT
	static inline void glProgramUniformMatrix3x2fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix3x2fvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix3x2fvEXT

#ifdef glProgramUniformMatrix3x4dv
#undef glProgramUniformMatrix3x4dv
	static inline void glProgramUniformMatrix3x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix3x4dv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix3x4dv

#ifdef glProgramUniformMatrix3x4dvEXT
#undef glProgramUniformMatrix3x4dvEXT
	static inline void glProgramUniformMatrix3x4dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix3x4dvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix3x4dvEXT

#ifdef glProgramUniformMatrix3x4fv
#undef glProgramUniformMatrix3x4fv
	static inline void glProgramUniformMatrix3x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix3x4fv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix3x4fv

#ifdef glProgramUniformMatrix3x4fvEXT
#undef glProgramUniformMatrix3x4fvEXT
	static inline void glProgramUniformMatrix3x4fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix3x4fvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix3x4fvEXT

#ifdef glProgramUniformMatrix4dv
#undef glProgramUniformMatrix4dv
	static inline void glProgramUniformMatrix4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix4dv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix4dv

#ifdef glProgramUniformMatrix4dvEXT
#undef glProgramUniformMatrix4dvEXT
	static inline void glProgramUniformMatrix4dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix4dvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix4dvEXT

#ifdef glProgramUniformMatrix4fv
#undef glProgramUniformMatrix4fv
	static inline void glProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix4fv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix4fv

#ifdef glProgramUniformMatrix4fvEXT
#undef glProgramUniformMatrix4fvEXT
	static inline void glProgramUniformMatrix4fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix4fvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix4fvEXT

#ifdef glProgramUniformMatrix4x2dv
#undef glProgramUniformMatrix4x2dv
	static inline void glProgramUniformMatrix4x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix4x2dv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix4x2dv

#ifdef glProgramUniformMatrix4x2dvEXT
#undef glProgramUniformMatrix4x2dvEXT
	static inline void glProgramUniformMatrix4x2dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix4x2dvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix4x2dvEXT

#ifdef glProgramUniformMatrix4x2fv
#undef glProgramUniformMatrix4x2fv
	static inline void glProgramUniformMatrix4x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix4x2fv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix4x2fv

#ifdef glProgramUniformMatrix4x2fvEXT
#undef glProgramUniformMatrix4x2fvEXT
	static inline void glProgramUniformMatrix4x2fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix4x2fvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix4x2fvEXT

#ifdef glProgramUniformMatrix4x3dv
#undef glProgramUniformMatrix4x3dv
	static inline void glProgramUniformMatrix4x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix4x3dv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix4x3dv

#ifdef glProgramUniformMatrix4x3dvEXT
#undef glProgramUniformMatrix4x3dvEXT
	static inline void glProgramUniformMatrix4x3dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glProgramUniformMatrix4x3dvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix4x3dvEXT

#ifdef glProgramUniformMatrix4x3fv
#undef glProgramUniformMatrix4x3fv
	static inline void glProgramUniformMatrix4x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix4x3fv(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix4x3fv

#ifdef glProgramUniformMatrix4x3fvEXT
#undef glProgramUniformMatrix4x3fvEXT
	static inline void glProgramUniformMatrix4x3fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glProgramUniformMatrix4x3fvEXT(program, location, count, transpose, value);
	}
#endif // glProgramUniformMatrix4x3fvEXT

#ifdef glProgramUniformui64NV
#undef glProgramUniformui64NV
	static inline void glProgramUniformui64NV(GLuint program, GLint location, GLuint64EXT value){
	    glad_debug_glProgramUniformui64NV(program, location, value);
	}
#endif // glProgramUniformui64NV

#ifdef glProgramUniformui64vNV
#undef glProgramUniformui64vNV
	static inline void glProgramUniformui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64EXT *value){
	    glad_debug_glProgramUniformui64vNV(program, location, count, value);
	}
#endif // glProgramUniformui64vNV

#ifdef glProgramVertexLimitNV
#undef glProgramVertexLimitNV
	static inline void glProgramVertexLimitNV(GLenum target, GLint limit){
	    glad_debug_glProgramVertexLimitNV(target, limit);
	}
#endif // glProgramVertexLimitNV

#ifdef glProvokingVertex
#undef glProvokingVertex
	static inline void glProvokingVertex(GLenum mode){
	    glad_debug_glProvokingVertex(mode);
	}
#endif // glProvokingVertex

#ifdef glProvokingVertexEXT
#undef glProvokingVertexEXT
	static inline void glProvokingVertexEXT(GLenum mode){
	    glad_debug_glProvokingVertexEXT(mode);
	}
#endif // glProvokingVertexEXT

#ifdef glPushAttrib
#undef glPushAttrib
	static inline void glPushAttrib(GLbitfield mask){
	    glad_debug_glPushAttrib(mask);
	}
#endif // glPushAttrib

#ifdef glPushClientAttrib
#undef glPushClientAttrib
	static inline void glPushClientAttrib(GLbitfield mask){
	    glad_debug_glPushClientAttrib(mask);
	}
#endif // glPushClientAttrib

#ifdef glPushClientAttribDefaultEXT
#undef glPushClientAttribDefaultEXT
	static inline void glPushClientAttribDefaultEXT(GLbitfield mask){
	    glad_debug_glPushClientAttribDefaultEXT(mask);
	}
#endif // glPushClientAttribDefaultEXT

#ifdef glPushDebugGroup
#undef glPushDebugGroup
	static inline void glPushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar *message){
	    glad_debug_glPushDebugGroup(source, id, length, message);
	}
#endif // glPushDebugGroup

#ifdef glPushDebugGroupKHR
#undef glPushDebugGroupKHR
	static inline void glPushDebugGroupKHR(GLenum source, GLuint id, GLsizei length, const GLchar *message){
	    glad_debug_glPushDebugGroupKHR(source, id, length, message);
	}
#endif // glPushDebugGroupKHR

#ifdef glPushGroupMarkerEXT
#undef glPushGroupMarkerEXT
	static inline void glPushGroupMarkerEXT(GLsizei length, const GLchar *marker){
	    glad_debug_glPushGroupMarkerEXT(length, marker);
	}
#endif // glPushGroupMarkerEXT

#ifdef glPushMatrix
#undef glPushMatrix
	static inline void glPushMatrix(){
	    glad_debug_glPushMatrix();
	}
#endif // glPushMatrix

#ifdef glPushName
#undef glPushName
	static inline void glPushName(GLuint name){
	    glad_debug_glPushName(name);
	}
#endif // glPushName

#ifdef glQueryCounter
#undef glQueryCounter
	static inline void glQueryCounter(GLuint id, GLenum target){
	    glad_debug_glQueryCounter(id, target);
	}
#endif // glQueryCounter

#ifdef glQueryCounterEXT
#undef glQueryCounterEXT
	static inline void glQueryCounterEXT(GLuint id, GLenum target){
	    glad_debug_glQueryCounterEXT(id, target);
	}
#endif // glQueryCounterEXT

#ifdef glQueryObjectParameteruiAMD
#undef glQueryObjectParameteruiAMD
	static inline void glQueryObjectParameteruiAMD(GLenum target, GLuint id, GLenum pname, GLuint param){
	    glad_debug_glQueryObjectParameteruiAMD(target, id, pname, param);
	}
#endif // glQueryObjectParameteruiAMD

#ifdef glQueryResourceTagNV
#undef glQueryResourceTagNV
	static inline void glQueryResourceTagNV(GLint tagId, const GLchar *tagString){
	    glad_debug_glQueryResourceTagNV(tagId, tagString);
	}
#endif // glQueryResourceTagNV

#ifdef glRasterPos2d
#undef glRasterPos2d
	static inline void glRasterPos2d(GLdouble x, GLdouble y){
	    glad_debug_glRasterPos2d(x, y);
	}
#endif // glRasterPos2d

#ifdef glRasterPos2dv
#undef glRasterPos2dv
	static inline void glRasterPos2dv(const GLdouble *v){
	    glad_debug_glRasterPos2dv(v);
	}
#endif // glRasterPos2dv

#ifdef glRasterPos2f
#undef glRasterPos2f
	static inline void glRasterPos2f(GLfloat x, GLfloat y){
	    glad_debug_glRasterPos2f(x, y);
	}
#endif // glRasterPos2f

#ifdef glRasterPos2fv
#undef glRasterPos2fv
	static inline void glRasterPos2fv(const GLfloat *v){
	    glad_debug_glRasterPos2fv(v);
	}
#endif // glRasterPos2fv

#ifdef glRasterPos2i
#undef glRasterPos2i
	static inline void glRasterPos2i(GLint x, GLint y){
	    glad_debug_glRasterPos2i(x, y);
	}
#endif // glRasterPos2i

#ifdef glRasterPos2iv
#undef glRasterPos2iv
	static inline void glRasterPos2iv(const GLint *v){
	    glad_debug_glRasterPos2iv(v);
	}
#endif // glRasterPos2iv

#ifdef glRasterPos2s
#undef glRasterPos2s
	static inline void glRasterPos2s(GLshort x, GLshort y){
	    glad_debug_glRasterPos2s(x, y);
	}
#endif // glRasterPos2s

#ifdef glRasterPos2sv
#undef glRasterPos2sv
	static inline void glRasterPos2sv(const GLshort *v){
	    glad_debug_glRasterPos2sv(v);
	}
#endif // glRasterPos2sv

#ifdef glRasterPos2xOES
#undef glRasterPos2xOES
	static inline void glRasterPos2xOES(GLfixed x, GLfixed y){
	    glad_debug_glRasterPos2xOES(x, y);
	}
#endif // glRasterPos2xOES

#ifdef glRasterPos2xvOES
#undef glRasterPos2xvOES
	static inline void glRasterPos2xvOES(const GLfixed *coords){
	    glad_debug_glRasterPos2xvOES(coords);
	}
#endif // glRasterPos2xvOES

#ifdef glRasterPos3d
#undef glRasterPos3d
	static inline void glRasterPos3d(GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glRasterPos3d(x, y, z);
	}
#endif // glRasterPos3d

#ifdef glRasterPos3dv
#undef glRasterPos3dv
	static inline void glRasterPos3dv(const GLdouble *v){
	    glad_debug_glRasterPos3dv(v);
	}
#endif // glRasterPos3dv

#ifdef glRasterPos3f
#undef glRasterPos3f
	static inline void glRasterPos3f(GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glRasterPos3f(x, y, z);
	}
#endif // glRasterPos3f

#ifdef glRasterPos3fv
#undef glRasterPos3fv
	static inline void glRasterPos3fv(const GLfloat *v){
	    glad_debug_glRasterPos3fv(v);
	}
#endif // glRasterPos3fv

#ifdef glRasterPos3i
#undef glRasterPos3i
	static inline void glRasterPos3i(GLint x, GLint y, GLint z){
	    glad_debug_glRasterPos3i(x, y, z);
	}
#endif // glRasterPos3i

#ifdef glRasterPos3iv
#undef glRasterPos3iv
	static inline void glRasterPos3iv(const GLint *v){
	    glad_debug_glRasterPos3iv(v);
	}
#endif // glRasterPos3iv

#ifdef glRasterPos3s
#undef glRasterPos3s
	static inline void glRasterPos3s(GLshort x, GLshort y, GLshort z){
	    glad_debug_glRasterPos3s(x, y, z);
	}
#endif // glRasterPos3s

#ifdef glRasterPos3sv
#undef glRasterPos3sv
	static inline void glRasterPos3sv(const GLshort *v){
	    glad_debug_glRasterPos3sv(v);
	}
#endif // glRasterPos3sv

#ifdef glRasterPos3xOES
#undef glRasterPos3xOES
	static inline void glRasterPos3xOES(GLfixed x, GLfixed y, GLfixed z){
	    glad_debug_glRasterPos3xOES(x, y, z);
	}
#endif // glRasterPos3xOES

#ifdef glRasterPos3xvOES
#undef glRasterPos3xvOES
	static inline void glRasterPos3xvOES(const GLfixed *coords){
	    glad_debug_glRasterPos3xvOES(coords);
	}
#endif // glRasterPos3xvOES

#ifdef glRasterPos4d
#undef glRasterPos4d
	static inline void glRasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glRasterPos4d(x, y, z, w);
	}
#endif // glRasterPos4d

#ifdef glRasterPos4dv
#undef glRasterPos4dv
	static inline void glRasterPos4dv(const GLdouble *v){
	    glad_debug_glRasterPos4dv(v);
	}
#endif // glRasterPos4dv

#ifdef glRasterPos4f
#undef glRasterPos4f
	static inline void glRasterPos4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glRasterPos4f(x, y, z, w);
	}
#endif // glRasterPos4f

#ifdef glRasterPos4fv
#undef glRasterPos4fv
	static inline void glRasterPos4fv(const GLfloat *v){
	    glad_debug_glRasterPos4fv(v);
	}
#endif // glRasterPos4fv

#ifdef glRasterPos4i
#undef glRasterPos4i
	static inline void glRasterPos4i(GLint x, GLint y, GLint z, GLint w){
	    glad_debug_glRasterPos4i(x, y, z, w);
	}
#endif // glRasterPos4i

#ifdef glRasterPos4iv
#undef glRasterPos4iv
	static inline void glRasterPos4iv(const GLint *v){
	    glad_debug_glRasterPos4iv(v);
	}
#endif // glRasterPos4iv

#ifdef glRasterPos4s
#undef glRasterPos4s
	static inline void glRasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w){
	    glad_debug_glRasterPos4s(x, y, z, w);
	}
#endif // glRasterPos4s

#ifdef glRasterPos4sv
#undef glRasterPos4sv
	static inline void glRasterPos4sv(const GLshort *v){
	    glad_debug_glRasterPos4sv(v);
	}
#endif // glRasterPos4sv

#ifdef glRasterPos4xOES
#undef glRasterPos4xOES
	static inline void glRasterPos4xOES(GLfixed x, GLfixed y, GLfixed z, GLfixed w){
	    glad_debug_glRasterPos4xOES(x, y, z, w);
	}
#endif // glRasterPos4xOES

#ifdef glRasterPos4xvOES
#undef glRasterPos4xvOES
	static inline void glRasterPos4xvOES(const GLfixed *coords){
	    glad_debug_glRasterPos4xvOES(coords);
	}
#endif // glRasterPos4xvOES

#ifdef glRasterSamplesEXT
#undef glRasterSamplesEXT
	static inline void glRasterSamplesEXT(GLuint samples, GLboolean fixedsamplelocations){
	    glad_debug_glRasterSamplesEXT(samples, fixedsamplelocations);
	}
#endif // glRasterSamplesEXT

#ifdef glReadBuffer
#undef glReadBuffer
	static inline void glReadBuffer(GLenum src){
	    glad_debug_glReadBuffer(src);
	}
#endif // glReadBuffer

#ifdef glReadBufferIndexedEXT
#undef glReadBufferIndexedEXT
	static inline void glReadBufferIndexedEXT(GLenum src, GLint index){
	    glad_debug_glReadBufferIndexedEXT(src, index);
	}
#endif // glReadBufferIndexedEXT

#ifdef glReadBufferNV
#undef glReadBufferNV
	static inline void glReadBufferNV(GLenum mode){
	    glad_debug_glReadBufferNV(mode);
	}
#endif // glReadBufferNV

#ifdef glReadInstrumentsSGIX
#undef glReadInstrumentsSGIX
	static inline void glReadInstrumentsSGIX(GLint marker){
	    glad_debug_glReadInstrumentsSGIX(marker);
	}
#endif // glReadInstrumentsSGIX

#ifdef glRectd
#undef glRectd
	static inline void glRectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2){
	    glad_debug_glRectd(x1, y1, x2, y2);
	}
#endif // glRectd

#ifdef glRectdv
#undef glRectdv
	static inline void glRectdv(const GLdouble *v1, const GLdouble *v2){
	    glad_debug_glRectdv(v1, v2);
	}
#endif // glRectdv

#ifdef glRectf
#undef glRectf
	static inline void glRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2){
	    glad_debug_glRectf(x1, y1, x2, y2);
	}
#endif // glRectf

#ifdef glRectfv
#undef glRectfv
	static inline void glRectfv(const GLfloat *v1, const GLfloat *v2){
	    glad_debug_glRectfv(v1, v2);
	}
#endif // glRectfv

#ifdef glRecti
#undef glRecti
	static inline void glRecti(GLint x1, GLint y1, GLint x2, GLint y2){
	    glad_debug_glRecti(x1, y1, x2, y2);
	}
#endif // glRecti

#ifdef glRectiv
#undef glRectiv
	static inline void glRectiv(const GLint *v1, const GLint *v2){
	    glad_debug_glRectiv(v1, v2);
	}
#endif // glRectiv

#ifdef glRects
#undef glRects
	static inline void glRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2){
	    glad_debug_glRects(x1, y1, x2, y2);
	}
#endif // glRects

#ifdef glRectsv
#undef glRectsv
	static inline void glRectsv(const GLshort *v1, const GLshort *v2){
	    glad_debug_glRectsv(v1, v2);
	}
#endif // glRectsv

#ifdef glRectxOES
#undef glRectxOES
	static inline void glRectxOES(GLfixed x1, GLfixed y1, GLfixed x2, GLfixed y2){
	    glad_debug_glRectxOES(x1, y1, x2, y2);
	}
#endif // glRectxOES

#ifdef glRectxvOES
#undef glRectxvOES
	static inline void glRectxvOES(const GLfixed *v1, const GLfixed *v2){
	    glad_debug_glRectxvOES(v1, v2);
	}
#endif // glRectxvOES

#ifdef glReferencePlaneSGIX
#undef glReferencePlaneSGIX
	static inline void glReferencePlaneSGIX(const GLdouble *equation){
	    glad_debug_glReferencePlaneSGIX(equation);
	}
#endif // glReferencePlaneSGIX

#ifdef glReleaseClientPointerRangeMESA
#undef glReleaseClientPointerRangeMESA
	static inline void* glReleaseClientPointerRangeMESA(GLsizeiptr *size){
		return     glad_debug_glReleaseClientPointerRangeMESA(size);
	}
#endif // glReleaseClientPointerRangeMESA

#ifdef glReleaseShaderCompiler
#undef glReleaseShaderCompiler
	static inline void glReleaseShaderCompiler(){
	    glad_debug_glReleaseShaderCompiler();
	}
#endif // glReleaseShaderCompiler

#ifdef glRenderGpuMaskNV
#undef glRenderGpuMaskNV
	static inline void glRenderGpuMaskNV(GLbitfield mask){
	    glad_debug_glRenderGpuMaskNV(mask);
	}
#endif // glRenderGpuMaskNV

#ifdef glRenderbufferStorage
#undef glRenderbufferStorage
	static inline void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glRenderbufferStorage(target, internalformat, width, height);
	}
#endif // glRenderbufferStorage

#ifdef glRenderbufferStorageEXT
#undef glRenderbufferStorageEXT
	static inline void glRenderbufferStorageEXT(GLenum target, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glRenderbufferStorageEXT(target, internalformat, width, height);
	}
#endif // glRenderbufferStorageEXT

#ifdef glRenderbufferStorageMultisample
#undef glRenderbufferStorageMultisample
	static inline void glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glRenderbufferStorageMultisample(target, samples, internalformat, width, height);
	}
#endif // glRenderbufferStorageMultisample

#ifdef glRenderbufferStorageMultisampleANGLE
#undef glRenderbufferStorageMultisampleANGLE
	static inline void glRenderbufferStorageMultisampleANGLE(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glRenderbufferStorageMultisampleANGLE(target, samples, internalformat, width, height);
	}
#endif // glRenderbufferStorageMultisampleANGLE

#ifdef glRenderbufferStorageMultisampleAPPLE
#undef glRenderbufferStorageMultisampleAPPLE
	static inline void glRenderbufferStorageMultisampleAPPLE(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glRenderbufferStorageMultisampleAPPLE(target, samples, internalformat, width, height);
	}
#endif // glRenderbufferStorageMultisampleAPPLE

#ifdef glRenderbufferStorageMultisampleAdvancedAMD
#undef glRenderbufferStorageMultisampleAdvancedAMD
	static inline void glRenderbufferStorageMultisampleAdvancedAMD(GLenum target, GLsizei samples, GLsizei storageSamples, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glRenderbufferStorageMultisampleAdvancedAMD(target, samples, storageSamples, internalformat, width, height);
	}
#endif // glRenderbufferStorageMultisampleAdvancedAMD

#ifdef glRenderbufferStorageMultisampleCoverageNV
#undef glRenderbufferStorageMultisampleCoverageNV
	static inline void glRenderbufferStorageMultisampleCoverageNV(GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glRenderbufferStorageMultisampleCoverageNV(target, coverageSamples, colorSamples, internalformat, width, height);
	}
#endif // glRenderbufferStorageMultisampleCoverageNV

#ifdef glRenderbufferStorageMultisampleEXT
#undef glRenderbufferStorageMultisampleEXT
	static inline void glRenderbufferStorageMultisampleEXT(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glRenderbufferStorageMultisampleEXT(target, samples, internalformat, width, height);
	}
#endif // glRenderbufferStorageMultisampleEXT

#ifdef glRenderbufferStorageMultisampleIMG
#undef glRenderbufferStorageMultisampleIMG
	static inline void glRenderbufferStorageMultisampleIMG(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glRenderbufferStorageMultisampleIMG(target, samples, internalformat, width, height);
	}
#endif // glRenderbufferStorageMultisampleIMG

#ifdef glRenderbufferStorageMultisampleNV
#undef glRenderbufferStorageMultisampleNV
	static inline void glRenderbufferStorageMultisampleNV(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glRenderbufferStorageMultisampleNV(target, samples, internalformat, width, height);
	}
#endif // glRenderbufferStorageMultisampleNV

#ifdef glRenderbufferStorageOES
#undef glRenderbufferStorageOES
	static inline void glRenderbufferStorageOES(GLenum target, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glRenderbufferStorageOES(target, internalformat, width, height);
	}
#endif // glRenderbufferStorageOES

#ifdef glReplacementCodeubSUN
#undef glReplacementCodeubSUN
	static inline void glReplacementCodeubSUN(GLubyte code){
	    glad_debug_glReplacementCodeubSUN(code);
	}
#endif // glReplacementCodeubSUN

#ifdef glReplacementCodeubvSUN
#undef glReplacementCodeubvSUN
	static inline void glReplacementCodeubvSUN(const GLubyte *code){
	    glad_debug_glReplacementCodeubvSUN(code);
	}
#endif // glReplacementCodeubvSUN

#ifdef glReplacementCodeuiColor3fVertex3fSUN
#undef glReplacementCodeuiColor3fVertex3fSUN
	static inline void glReplacementCodeuiColor3fVertex3fSUN(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glReplacementCodeuiColor3fVertex3fSUN(rc, r, g, b, x, y, z);
	}
#endif // glReplacementCodeuiColor3fVertex3fSUN

#ifdef glReplacementCodeuiColor3fVertex3fvSUN
#undef glReplacementCodeuiColor3fVertex3fvSUN
	static inline void glReplacementCodeuiColor3fVertex3fvSUN(const GLuint *rc, const GLfloat *c, const GLfloat *v){
	    glad_debug_glReplacementCodeuiColor3fVertex3fvSUN(rc, c, v);
	}
#endif // glReplacementCodeuiColor3fVertex3fvSUN

#ifdef glReplacementCodeuiColor4fNormal3fVertex3fSUN
#undef glReplacementCodeuiColor4fNormal3fVertex3fSUN
	static inline void glReplacementCodeuiColor4fNormal3fVertex3fSUN(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glReplacementCodeuiColor4fNormal3fVertex3fSUN(rc, r, g, b, a, nx, ny, nz, x, y, z);
	}
#endif // glReplacementCodeuiColor4fNormal3fVertex3fSUN

#ifdef glReplacementCodeuiColor4fNormal3fVertex3fvSUN
#undef glReplacementCodeuiColor4fNormal3fVertex3fvSUN
	static inline void glReplacementCodeuiColor4fNormal3fVertex3fvSUN(const GLuint *rc, const GLfloat *c, const GLfloat *n, const GLfloat *v){
	    glad_debug_glReplacementCodeuiColor4fNormal3fVertex3fvSUN(rc, c, n, v);
	}
#endif // glReplacementCodeuiColor4fNormal3fVertex3fvSUN

#ifdef glReplacementCodeuiColor4ubVertex3fSUN
#undef glReplacementCodeuiColor4ubVertex3fSUN
	static inline void glReplacementCodeuiColor4ubVertex3fSUN(GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glReplacementCodeuiColor4ubVertex3fSUN(rc, r, g, b, a, x, y, z);
	}
#endif // glReplacementCodeuiColor4ubVertex3fSUN

#ifdef glReplacementCodeuiColor4ubVertex3fvSUN
#undef glReplacementCodeuiColor4ubVertex3fvSUN
	static inline void glReplacementCodeuiColor4ubVertex3fvSUN(const GLuint *rc, const GLubyte *c, const GLfloat *v){
	    glad_debug_glReplacementCodeuiColor4ubVertex3fvSUN(rc, c, v);
	}
#endif // glReplacementCodeuiColor4ubVertex3fvSUN

#ifdef glReplacementCodeuiNormal3fVertex3fSUN
#undef glReplacementCodeuiNormal3fVertex3fSUN
	static inline void glReplacementCodeuiNormal3fVertex3fSUN(GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glReplacementCodeuiNormal3fVertex3fSUN(rc, nx, ny, nz, x, y, z);
	}
#endif // glReplacementCodeuiNormal3fVertex3fSUN

#ifdef glReplacementCodeuiNormal3fVertex3fvSUN
#undef glReplacementCodeuiNormal3fVertex3fvSUN
	static inline void glReplacementCodeuiNormal3fVertex3fvSUN(const GLuint *rc, const GLfloat *n, const GLfloat *v){
	    glad_debug_glReplacementCodeuiNormal3fVertex3fvSUN(rc, n, v);
	}
#endif // glReplacementCodeuiNormal3fVertex3fvSUN

#ifdef glReplacementCodeuiSUN
#undef glReplacementCodeuiSUN
	static inline void glReplacementCodeuiSUN(GLuint code){
	    glad_debug_glReplacementCodeuiSUN(code);
	}
#endif // glReplacementCodeuiSUN

#ifdef glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN
#undef glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN
	static inline void glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN(GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN(rc, s, t, r, g, b, a, nx, ny, nz, x, y, z);
	}
#endif // glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN

#ifdef glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN
#undef glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN
	static inline void glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN(const GLuint *rc, const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v){
	    glad_debug_glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN(rc, tc, c, n, v);
	}
#endif // glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN

#ifdef glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN
#undef glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN
	static inline void glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN(GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN(rc, s, t, nx, ny, nz, x, y, z);
	}
#endif // glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN

#ifdef glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN
#undef glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN
	static inline void glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN(const GLuint *rc, const GLfloat *tc, const GLfloat *n, const GLfloat *v){
	    glad_debug_glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN(rc, tc, n, v);
	}
#endif // glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN

#ifdef glReplacementCodeuiTexCoord2fVertex3fSUN
#undef glReplacementCodeuiTexCoord2fVertex3fSUN
	static inline void glReplacementCodeuiTexCoord2fVertex3fSUN(GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glReplacementCodeuiTexCoord2fVertex3fSUN(rc, s, t, x, y, z);
	}
#endif // glReplacementCodeuiTexCoord2fVertex3fSUN

#ifdef glReplacementCodeuiTexCoord2fVertex3fvSUN
#undef glReplacementCodeuiTexCoord2fVertex3fvSUN
	static inline void glReplacementCodeuiTexCoord2fVertex3fvSUN(const GLuint *rc, const GLfloat *tc, const GLfloat *v){
	    glad_debug_glReplacementCodeuiTexCoord2fVertex3fvSUN(rc, tc, v);
	}
#endif // glReplacementCodeuiTexCoord2fVertex3fvSUN

#ifdef glReplacementCodeuiVertex3fSUN
#undef glReplacementCodeuiVertex3fSUN
	static inline void glReplacementCodeuiVertex3fSUN(GLuint rc, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glReplacementCodeuiVertex3fSUN(rc, x, y, z);
	}
#endif // glReplacementCodeuiVertex3fSUN

#ifdef glReplacementCodeuiVertex3fvSUN
#undef glReplacementCodeuiVertex3fvSUN
	static inline void glReplacementCodeuiVertex3fvSUN(const GLuint *rc, const GLfloat *v){
	    glad_debug_glReplacementCodeuiVertex3fvSUN(rc, v);
	}
#endif // glReplacementCodeuiVertex3fvSUN

#ifdef glReplacementCodeuivSUN
#undef glReplacementCodeuivSUN
	static inline void glReplacementCodeuivSUN(const GLuint *code){
	    glad_debug_glReplacementCodeuivSUN(code);
	}
#endif // glReplacementCodeuivSUN

#ifdef glReplacementCodeusSUN
#undef glReplacementCodeusSUN
	static inline void glReplacementCodeusSUN(GLushort code){
	    glad_debug_glReplacementCodeusSUN(code);
	}
#endif // glReplacementCodeusSUN

#ifdef glReplacementCodeusvSUN
#undef glReplacementCodeusvSUN
	static inline void glReplacementCodeusvSUN(const GLushort *code){
	    glad_debug_glReplacementCodeusvSUN(code);
	}
#endif // glReplacementCodeusvSUN

#ifdef glRequestResidentProgramsNV
#undef glRequestResidentProgramsNV
	static inline void glRequestResidentProgramsNV(GLsizei n, const GLuint *programs){
	    glad_debug_glRequestResidentProgramsNV(n, programs);
	}
#endif // glRequestResidentProgramsNV

#ifdef glResetHistogram
#undef glResetHistogram
	static inline void glResetHistogram(GLenum target){
	    glad_debug_glResetHistogram(target);
	}
#endif // glResetHistogram

#ifdef glResetHistogramEXT
#undef glResetHistogramEXT
	static inline void glResetHistogramEXT(GLenum target){
	    glad_debug_glResetHistogramEXT(target);
	}
#endif // glResetHistogramEXT

#ifdef glResetMemoryObjectParameterNV
#undef glResetMemoryObjectParameterNV
	static inline void glResetMemoryObjectParameterNV(GLuint memory, GLenum pname){
	    glad_debug_glResetMemoryObjectParameterNV(memory, pname);
	}
#endif // glResetMemoryObjectParameterNV

#ifdef glResetMinmax
#undef glResetMinmax
	static inline void glResetMinmax(GLenum target){
	    glad_debug_glResetMinmax(target);
	}
#endif // glResetMinmax

#ifdef glResetMinmaxEXT
#undef glResetMinmaxEXT
	static inline void glResetMinmaxEXT(GLenum target){
	    glad_debug_glResetMinmaxEXT(target);
	}
#endif // glResetMinmaxEXT

#ifdef glResizeBuffersMESA
#undef glResizeBuffersMESA
	static inline void glResizeBuffersMESA(){
	    glad_debug_glResizeBuffersMESA();
	}
#endif // glResizeBuffersMESA

#ifdef glResolveDepthValuesNV
#undef glResolveDepthValuesNV
	static inline void glResolveDepthValuesNV(){
	    glad_debug_glResolveDepthValuesNV();
	}
#endif // glResolveDepthValuesNV

#ifdef glResolveMultisampleFramebufferAPPLE
#undef glResolveMultisampleFramebufferAPPLE
	static inline void glResolveMultisampleFramebufferAPPLE(){
	    glad_debug_glResolveMultisampleFramebufferAPPLE();
	}
#endif // glResolveMultisampleFramebufferAPPLE

#ifdef glResumeTransformFeedback
#undef glResumeTransformFeedback
	static inline void glResumeTransformFeedback(){
	    glad_debug_glResumeTransformFeedback();
	}
#endif // glResumeTransformFeedback

#ifdef glResumeTransformFeedbackNV
#undef glResumeTransformFeedbackNV
	static inline void glResumeTransformFeedbackNV(){
	    glad_debug_glResumeTransformFeedbackNV();
	}
#endif // glResumeTransformFeedbackNV

#ifdef glRotated
#undef glRotated
	static inline void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glRotated(angle, x, y, z);
	}
#endif // glRotated

#ifdef glRotatef
#undef glRotatef
	static inline void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glRotatef(angle, x, y, z);
	}
#endif // glRotatef

#ifdef glRotatex
#undef glRotatex
	static inline void glRotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z){
	    glad_debug_glRotatex(angle, x, y, z);
	}
#endif // glRotatex

#ifdef glRotatexOES
#undef glRotatexOES
	static inline void glRotatexOES(GLfixed angle, GLfixed x, GLfixed y, GLfixed z){
	    glad_debug_glRotatexOES(angle, x, y, z);
	}
#endif // glRotatexOES

#ifdef glSampleCoverage
#undef glSampleCoverage
	static inline void glSampleCoverage(GLfloat value, GLboolean invert){
	    glad_debug_glSampleCoverage(value, invert);
	}
#endif // glSampleCoverage

#ifdef glSampleCoverageARB
#undef glSampleCoverageARB
	static inline void glSampleCoverageARB(GLfloat value, GLboolean invert){
	    glad_debug_glSampleCoverageARB(value, invert);
	}
#endif // glSampleCoverageARB

#ifdef glSampleCoveragex
#undef glSampleCoveragex
	static inline void glSampleCoveragex(GLclampx value, GLboolean invert){
	    glad_debug_glSampleCoveragex(value, invert);
	}
#endif // glSampleCoveragex

#ifdef glSampleCoveragexOES
#undef glSampleCoveragexOES
	static inline void glSampleCoveragexOES(GLclampx value, GLboolean invert){
	    glad_debug_glSampleCoveragexOES(value, invert);
	}
#endif // glSampleCoveragexOES

#ifdef glSampleMapATI
#undef glSampleMapATI
	static inline void glSampleMapATI(GLuint dst, GLuint interp, GLenum swizzle){
	    glad_debug_glSampleMapATI(dst, interp, swizzle);
	}
#endif // glSampleMapATI

#ifdef glSampleMaskEXT
#undef glSampleMaskEXT
	static inline void glSampleMaskEXT(GLclampf value, GLboolean invert){
	    glad_debug_glSampleMaskEXT(value, invert);
	}
#endif // glSampleMaskEXT

#ifdef glSampleMaskIndexedNV
#undef glSampleMaskIndexedNV
	static inline void glSampleMaskIndexedNV(GLuint index, GLbitfield mask){
	    glad_debug_glSampleMaskIndexedNV(index, mask);
	}
#endif // glSampleMaskIndexedNV

#ifdef glSampleMaskSGIS
#undef glSampleMaskSGIS
	static inline void glSampleMaskSGIS(GLclampf value, GLboolean invert){
	    glad_debug_glSampleMaskSGIS(value, invert);
	}
#endif // glSampleMaskSGIS

#ifdef glSampleMaski
#undef glSampleMaski
	static inline void glSampleMaski(GLuint maskNumber, GLbitfield mask){
	    glad_debug_glSampleMaski(maskNumber, mask);
	}
#endif // glSampleMaski

#ifdef glSamplePatternEXT
#undef glSamplePatternEXT
	static inline void glSamplePatternEXT(GLenum pattern){
	    glad_debug_glSamplePatternEXT(pattern);
	}
#endif // glSamplePatternEXT

#ifdef glSamplePatternSGIS
#undef glSamplePatternSGIS
	static inline void glSamplePatternSGIS(GLenum pattern){
	    glad_debug_glSamplePatternSGIS(pattern);
	}
#endif // glSamplePatternSGIS

#ifdef glSamplerParameterIiv
#undef glSamplerParameterIiv
	static inline void glSamplerParameterIiv(GLuint sampler, GLenum pname, const GLint *param){
	    glad_debug_glSamplerParameterIiv(sampler, pname, param);
	}
#endif // glSamplerParameterIiv

#ifdef glSamplerParameterIivEXT
#undef glSamplerParameterIivEXT
	static inline void glSamplerParameterIivEXT(GLuint sampler, GLenum pname, const GLint *param){
	    glad_debug_glSamplerParameterIivEXT(sampler, pname, param);
	}
#endif // glSamplerParameterIivEXT

#ifdef glSamplerParameterIivOES
#undef glSamplerParameterIivOES
	static inline void glSamplerParameterIivOES(GLuint sampler, GLenum pname, const GLint *param){
	    glad_debug_glSamplerParameterIivOES(sampler, pname, param);
	}
#endif // glSamplerParameterIivOES

#ifdef glSamplerParameterIuiv
#undef glSamplerParameterIuiv
	static inline void glSamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint *param){
	    glad_debug_glSamplerParameterIuiv(sampler, pname, param);
	}
#endif // glSamplerParameterIuiv

#ifdef glSamplerParameterIuivEXT
#undef glSamplerParameterIuivEXT
	static inline void glSamplerParameterIuivEXT(GLuint sampler, GLenum pname, const GLuint *param){
	    glad_debug_glSamplerParameterIuivEXT(sampler, pname, param);
	}
#endif // glSamplerParameterIuivEXT

#ifdef glSamplerParameterIuivOES
#undef glSamplerParameterIuivOES
	static inline void glSamplerParameterIuivOES(GLuint sampler, GLenum pname, const GLuint *param){
	    glad_debug_glSamplerParameterIuivOES(sampler, pname, param);
	}
#endif // glSamplerParameterIuivOES

#ifdef glSamplerParameterf
#undef glSamplerParameterf
	static inline void glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param){
	    glad_debug_glSamplerParameterf(sampler, pname, param);
	}
#endif // glSamplerParameterf

#ifdef glSamplerParameterfv
#undef glSamplerParameterfv
	static inline void glSamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat *param){
	    glad_debug_glSamplerParameterfv(sampler, pname, param);
	}
#endif // glSamplerParameterfv

#ifdef glSamplerParameteri
#undef glSamplerParameteri
	static inline void glSamplerParameteri(GLuint sampler, GLenum pname, GLint param){
	    glad_debug_glSamplerParameteri(sampler, pname, param);
	}
#endif // glSamplerParameteri

#ifdef glSamplerParameteriv
#undef glSamplerParameteriv
	static inline void glSamplerParameteriv(GLuint sampler, GLenum pname, const GLint *param){
	    glad_debug_glSamplerParameteriv(sampler, pname, param);
	}
#endif // glSamplerParameteriv

#ifdef glScaled
#undef glScaled
	static inline void glScaled(GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glScaled(x, y, z);
	}
#endif // glScaled

#ifdef glScalef
#undef glScalef
	static inline void glScalef(GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glScalef(x, y, z);
	}
#endif // glScalef

#ifdef glScalex
#undef glScalex
	static inline void glScalex(GLfixed x, GLfixed y, GLfixed z){
	    glad_debug_glScalex(x, y, z);
	}
#endif // glScalex

#ifdef glScalexOES
#undef glScalexOES
	static inline void glScalexOES(GLfixed x, GLfixed y, GLfixed z){
	    glad_debug_glScalexOES(x, y, z);
	}
#endif // glScalexOES

#ifdef glScissor
#undef glScissor
	static inline void glScissor(GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glScissor(x, y, width, height);
	}
#endif // glScissor

#ifdef glScissorArrayv
#undef glScissorArrayv
	static inline void glScissorArrayv(GLuint first, GLsizei count, const GLint *v){
	    glad_debug_glScissorArrayv(first, count, v);
	}
#endif // glScissorArrayv

#ifdef glScissorArrayvNV
#undef glScissorArrayvNV
	static inline void glScissorArrayvNV(GLuint first, GLsizei count, const GLint *v){
	    glad_debug_glScissorArrayvNV(first, count, v);
	}
#endif // glScissorArrayvNV

#ifdef glScissorArrayvOES
#undef glScissorArrayvOES
	static inline void glScissorArrayvOES(GLuint first, GLsizei count, const GLint *v){
	    glad_debug_glScissorArrayvOES(first, count, v);
	}
#endif // glScissorArrayvOES

#ifdef glScissorExclusiveArrayvNV
#undef glScissorExclusiveArrayvNV
	static inline void glScissorExclusiveArrayvNV(GLuint first, GLsizei count, const GLint *v){
	    glad_debug_glScissorExclusiveArrayvNV(first, count, v);
	}
#endif // glScissorExclusiveArrayvNV

#ifdef glScissorExclusiveNV
#undef glScissorExclusiveNV
	static inline void glScissorExclusiveNV(GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glScissorExclusiveNV(x, y, width, height);
	}
#endif // glScissorExclusiveNV

#ifdef glScissorIndexed
#undef glScissorIndexed
	static inline void glScissorIndexed(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height){
	    glad_debug_glScissorIndexed(index, left, bottom, width, height);
	}
#endif // glScissorIndexed

#ifdef glScissorIndexedNV
#undef glScissorIndexedNV
	static inline void glScissorIndexedNV(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height){
	    glad_debug_glScissorIndexedNV(index, left, bottom, width, height);
	}
#endif // glScissorIndexedNV

#ifdef glScissorIndexedOES
#undef glScissorIndexedOES
	static inline void glScissorIndexedOES(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height){
	    glad_debug_glScissorIndexedOES(index, left, bottom, width, height);
	}
#endif // glScissorIndexedOES

#ifdef glScissorIndexedv
#undef glScissorIndexedv
	static inline void glScissorIndexedv(GLuint index, const GLint *v){
	    glad_debug_glScissorIndexedv(index, v);
	}
#endif // glScissorIndexedv

#ifdef glScissorIndexedvNV
#undef glScissorIndexedvNV
	static inline void glScissorIndexedvNV(GLuint index, const GLint *v){
	    glad_debug_glScissorIndexedvNV(index, v);
	}
#endif // glScissorIndexedvNV

#ifdef glScissorIndexedvOES
#undef glScissorIndexedvOES
	static inline void glScissorIndexedvOES(GLuint index, const GLint *v){
	    glad_debug_glScissorIndexedvOES(index, v);
	}
#endif // glScissorIndexedvOES

#ifdef glSecondaryColor3b
#undef glSecondaryColor3b
	static inline void glSecondaryColor3b(GLbyte red, GLbyte green, GLbyte blue){
	    glad_debug_glSecondaryColor3b(red, green, blue);
	}
#endif // glSecondaryColor3b

#ifdef glSecondaryColor3bEXT
#undef glSecondaryColor3bEXT
	static inline void glSecondaryColor3bEXT(GLbyte red, GLbyte green, GLbyte blue){
	    glad_debug_glSecondaryColor3bEXT(red, green, blue);
	}
#endif // glSecondaryColor3bEXT

#ifdef glSecondaryColor3bv
#undef glSecondaryColor3bv
	static inline void glSecondaryColor3bv(const GLbyte *v){
	    glad_debug_glSecondaryColor3bv(v);
	}
#endif // glSecondaryColor3bv

#ifdef glSecondaryColor3bvEXT
#undef glSecondaryColor3bvEXT
	static inline void glSecondaryColor3bvEXT(const GLbyte *v){
	    glad_debug_glSecondaryColor3bvEXT(v);
	}
#endif // glSecondaryColor3bvEXT

#ifdef glSecondaryColor3d
#undef glSecondaryColor3d
	static inline void glSecondaryColor3d(GLdouble red, GLdouble green, GLdouble blue){
	    glad_debug_glSecondaryColor3d(red, green, blue);
	}
#endif // glSecondaryColor3d

#ifdef glSecondaryColor3dEXT
#undef glSecondaryColor3dEXT
	static inline void glSecondaryColor3dEXT(GLdouble red, GLdouble green, GLdouble blue){
	    glad_debug_glSecondaryColor3dEXT(red, green, blue);
	}
#endif // glSecondaryColor3dEXT

#ifdef glSecondaryColor3dv
#undef glSecondaryColor3dv
	static inline void glSecondaryColor3dv(const GLdouble *v){
	    glad_debug_glSecondaryColor3dv(v);
	}
#endif // glSecondaryColor3dv

#ifdef glSecondaryColor3dvEXT
#undef glSecondaryColor3dvEXT
	static inline void glSecondaryColor3dvEXT(const GLdouble *v){
	    glad_debug_glSecondaryColor3dvEXT(v);
	}
#endif // glSecondaryColor3dvEXT

#ifdef glSecondaryColor3f
#undef glSecondaryColor3f
	static inline void glSecondaryColor3f(GLfloat red, GLfloat green, GLfloat blue){
	    glad_debug_glSecondaryColor3f(red, green, blue);
	}
#endif // glSecondaryColor3f

#ifdef glSecondaryColor3fEXT
#undef glSecondaryColor3fEXT
	static inline void glSecondaryColor3fEXT(GLfloat red, GLfloat green, GLfloat blue){
	    glad_debug_glSecondaryColor3fEXT(red, green, blue);
	}
#endif // glSecondaryColor3fEXT

#ifdef glSecondaryColor3fv
#undef glSecondaryColor3fv
	static inline void glSecondaryColor3fv(const GLfloat *v){
	    glad_debug_glSecondaryColor3fv(v);
	}
#endif // glSecondaryColor3fv

#ifdef glSecondaryColor3fvEXT
#undef glSecondaryColor3fvEXT
	static inline void glSecondaryColor3fvEXT(const GLfloat *v){
	    glad_debug_glSecondaryColor3fvEXT(v);
	}
#endif // glSecondaryColor3fvEXT

#ifdef glSecondaryColor3hNV
#undef glSecondaryColor3hNV
	static inline void glSecondaryColor3hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue){
	    glad_debug_glSecondaryColor3hNV(red, green, blue);
	}
#endif // glSecondaryColor3hNV

#ifdef glSecondaryColor3hvNV
#undef glSecondaryColor3hvNV
	static inline void glSecondaryColor3hvNV(const GLhalfNV *v){
	    glad_debug_glSecondaryColor3hvNV(v);
	}
#endif // glSecondaryColor3hvNV

#ifdef glSecondaryColor3i
#undef glSecondaryColor3i
	static inline void glSecondaryColor3i(GLint red, GLint green, GLint blue){
	    glad_debug_glSecondaryColor3i(red, green, blue);
	}
#endif // glSecondaryColor3i

#ifdef glSecondaryColor3iEXT
#undef glSecondaryColor3iEXT
	static inline void glSecondaryColor3iEXT(GLint red, GLint green, GLint blue){
	    glad_debug_glSecondaryColor3iEXT(red, green, blue);
	}
#endif // glSecondaryColor3iEXT

#ifdef glSecondaryColor3iv
#undef glSecondaryColor3iv
	static inline void glSecondaryColor3iv(const GLint *v){
	    glad_debug_glSecondaryColor3iv(v);
	}
#endif // glSecondaryColor3iv

#ifdef glSecondaryColor3ivEXT
#undef glSecondaryColor3ivEXT
	static inline void glSecondaryColor3ivEXT(const GLint *v){
	    glad_debug_glSecondaryColor3ivEXT(v);
	}
#endif // glSecondaryColor3ivEXT

#ifdef glSecondaryColor3s
#undef glSecondaryColor3s
	static inline void glSecondaryColor3s(GLshort red, GLshort green, GLshort blue){
	    glad_debug_glSecondaryColor3s(red, green, blue);
	}
#endif // glSecondaryColor3s

#ifdef glSecondaryColor3sEXT
#undef glSecondaryColor3sEXT
	static inline void glSecondaryColor3sEXT(GLshort red, GLshort green, GLshort blue){
	    glad_debug_glSecondaryColor3sEXT(red, green, blue);
	}
#endif // glSecondaryColor3sEXT

#ifdef glSecondaryColor3sv
#undef glSecondaryColor3sv
	static inline void glSecondaryColor3sv(const GLshort *v){
	    glad_debug_glSecondaryColor3sv(v);
	}
#endif // glSecondaryColor3sv

#ifdef glSecondaryColor3svEXT
#undef glSecondaryColor3svEXT
	static inline void glSecondaryColor3svEXT(const GLshort *v){
	    glad_debug_glSecondaryColor3svEXT(v);
	}
#endif // glSecondaryColor3svEXT

#ifdef glSecondaryColor3ub
#undef glSecondaryColor3ub
	static inline void glSecondaryColor3ub(GLubyte red, GLubyte green, GLubyte blue){
	    glad_debug_glSecondaryColor3ub(red, green, blue);
	}
#endif // glSecondaryColor3ub

#ifdef glSecondaryColor3ubEXT
#undef glSecondaryColor3ubEXT
	static inline void glSecondaryColor3ubEXT(GLubyte red, GLubyte green, GLubyte blue){
	    glad_debug_glSecondaryColor3ubEXT(red, green, blue);
	}
#endif // glSecondaryColor3ubEXT

#ifdef glSecondaryColor3ubv
#undef glSecondaryColor3ubv
	static inline void glSecondaryColor3ubv(const GLubyte *v){
	    glad_debug_glSecondaryColor3ubv(v);
	}
#endif // glSecondaryColor3ubv

#ifdef glSecondaryColor3ubvEXT
#undef glSecondaryColor3ubvEXT
	static inline void glSecondaryColor3ubvEXT(const GLubyte *v){
	    glad_debug_glSecondaryColor3ubvEXT(v);
	}
#endif // glSecondaryColor3ubvEXT

#ifdef glSecondaryColor3ui
#undef glSecondaryColor3ui
	static inline void glSecondaryColor3ui(GLuint red, GLuint green, GLuint blue){
	    glad_debug_glSecondaryColor3ui(red, green, blue);
	}
#endif // glSecondaryColor3ui

#ifdef glSecondaryColor3uiEXT
#undef glSecondaryColor3uiEXT
	static inline void glSecondaryColor3uiEXT(GLuint red, GLuint green, GLuint blue){
	    glad_debug_glSecondaryColor3uiEXT(red, green, blue);
	}
#endif // glSecondaryColor3uiEXT

#ifdef glSecondaryColor3uiv
#undef glSecondaryColor3uiv
	static inline void glSecondaryColor3uiv(const GLuint *v){
	    glad_debug_glSecondaryColor3uiv(v);
	}
#endif // glSecondaryColor3uiv

#ifdef glSecondaryColor3uivEXT
#undef glSecondaryColor3uivEXT
	static inline void glSecondaryColor3uivEXT(const GLuint *v){
	    glad_debug_glSecondaryColor3uivEXT(v);
	}
#endif // glSecondaryColor3uivEXT

#ifdef glSecondaryColor3us
#undef glSecondaryColor3us
	static inline void glSecondaryColor3us(GLushort red, GLushort green, GLushort blue){
	    glad_debug_glSecondaryColor3us(red, green, blue);
	}
#endif // glSecondaryColor3us

#ifdef glSecondaryColor3usEXT
#undef glSecondaryColor3usEXT
	static inline void glSecondaryColor3usEXT(GLushort red, GLushort green, GLushort blue){
	    glad_debug_glSecondaryColor3usEXT(red, green, blue);
	}
#endif // glSecondaryColor3usEXT

#ifdef glSecondaryColor3usv
#undef glSecondaryColor3usv
	static inline void glSecondaryColor3usv(const GLushort *v){
	    glad_debug_glSecondaryColor3usv(v);
	}
#endif // glSecondaryColor3usv

#ifdef glSecondaryColor3usvEXT
#undef glSecondaryColor3usvEXT
	static inline void glSecondaryColor3usvEXT(const GLushort *v){
	    glad_debug_glSecondaryColor3usvEXT(v);
	}
#endif // glSecondaryColor3usvEXT

#ifdef glSecondaryColorFormatNV
#undef glSecondaryColorFormatNV
	static inline void glSecondaryColorFormatNV(GLint size, GLenum type, GLsizei stride){
	    glad_debug_glSecondaryColorFormatNV(size, type, stride);
	}
#endif // glSecondaryColorFormatNV

#ifdef glSecondaryColorP3ui
#undef glSecondaryColorP3ui
	static inline void glSecondaryColorP3ui(GLenum type, GLuint color){
	    glad_debug_glSecondaryColorP3ui(type, color);
	}
#endif // glSecondaryColorP3ui

#ifdef glSecondaryColorP3uiv
#undef glSecondaryColorP3uiv
	static inline void glSecondaryColorP3uiv(GLenum type, const GLuint *color){
	    glad_debug_glSecondaryColorP3uiv(type, color);
	}
#endif // glSecondaryColorP3uiv

#ifdef glSelectBuffer
#undef glSelectBuffer
	static inline void glSelectBuffer(GLsizei size, GLuint *buffer){
	    glad_debug_glSelectBuffer(size, buffer);
	}
#endif // glSelectBuffer

#ifdef glSelectPerfMonitorCountersAMD
#undef glSelectPerfMonitorCountersAMD
	static inline void glSelectPerfMonitorCountersAMD(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint *counterList){
	    glad_debug_glSelectPerfMonitorCountersAMD(monitor, enable, group, numCounters, counterList);
	}
#endif // glSelectPerfMonitorCountersAMD

#ifdef glSemaphoreParameterivNV
#undef glSemaphoreParameterivNV
	static inline void glSemaphoreParameterivNV(GLuint semaphore, GLenum pname, const GLint *params){
	    glad_debug_glSemaphoreParameterivNV(semaphore, pname, params);
	}
#endif // glSemaphoreParameterivNV

#ifdef glSemaphoreParameterui64vEXT
#undef glSemaphoreParameterui64vEXT
	static inline void glSemaphoreParameterui64vEXT(GLuint semaphore, GLenum pname, const GLuint64 *params){
	    glad_debug_glSemaphoreParameterui64vEXT(semaphore, pname, params);
	}
#endif // glSemaphoreParameterui64vEXT

#ifdef glSetFenceAPPLE
#undef glSetFenceAPPLE
	static inline void glSetFenceAPPLE(GLuint fence){
	    glad_debug_glSetFenceAPPLE(fence);
	}
#endif // glSetFenceAPPLE

#ifdef glSetFenceNV
#undef glSetFenceNV
	static inline void glSetFenceNV(GLuint fence, GLenum condition){
	    glad_debug_glSetFenceNV(fence, condition);
	}
#endif // glSetFenceNV

#ifdef glSetFragmentShaderConstantATI
#undef glSetFragmentShaderConstantATI
	static inline void glSetFragmentShaderConstantATI(GLuint dst, const GLfloat *value){
	    glad_debug_glSetFragmentShaderConstantATI(dst, value);
	}
#endif // glSetFragmentShaderConstantATI

#ifdef glSetMultisamplefvAMD
#undef glSetMultisamplefvAMD
	static inline void glSetMultisamplefvAMD(GLenum pname, GLuint index, const GLfloat *val){
	    glad_debug_glSetMultisamplefvAMD(pname, index, val);
	}
#endif // glSetMultisamplefvAMD

#ifdef glShadeModel
#undef glShadeModel
	static inline void glShadeModel(GLenum mode){
	    glad_debug_glShadeModel(mode);
	}
#endif // glShadeModel

#ifdef glShaderOp1EXT
#undef glShaderOp1EXT
	static inline void glShaderOp1EXT(GLenum op, GLuint res, GLuint arg1){
	    glad_debug_glShaderOp1EXT(op, res, arg1);
	}
#endif // glShaderOp1EXT

#ifdef glShaderOp2EXT
#undef glShaderOp2EXT
	static inline void glShaderOp2EXT(GLenum op, GLuint res, GLuint arg1, GLuint arg2){
	    glad_debug_glShaderOp2EXT(op, res, arg1, arg2);
	}
#endif // glShaderOp2EXT

#ifdef glShaderOp3EXT
#undef glShaderOp3EXT
	static inline void glShaderOp3EXT(GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3){
	    glad_debug_glShaderOp3EXT(op, res, arg1, arg2, arg3);
	}
#endif // glShaderOp3EXT

#ifdef glShaderSource
#undef glShaderSource
	static inline void glShaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length){
	    glad_debug_glShaderSource(shader, count, string, length);
	}
#endif // glShaderSource

#ifdef glShaderSourceARB
#undef glShaderSourceARB
	static inline void glShaderSourceARB(GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length){
	    glad_debug_glShaderSourceARB(shaderObj, count, string, length);
	}
#endif // glShaderSourceARB

#ifdef glShaderStorageBlockBinding
#undef glShaderStorageBlockBinding
	static inline void glShaderStorageBlockBinding(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding){
	    glad_debug_glShaderStorageBlockBinding(program, storageBlockIndex, storageBlockBinding);
	}
#endif // glShaderStorageBlockBinding

#ifdef glShadingRateEXT
#undef glShadingRateEXT
	static inline void glShadingRateEXT(GLenum rate){
	    glad_debug_glShadingRateEXT(rate);
	}
#endif // glShadingRateEXT

#ifdef glShadingRateCombinerOpsEXT
#undef glShadingRateCombinerOpsEXT
	static inline void glShadingRateCombinerOpsEXT(GLenum combinerOp0, GLenum combinerOp1){
	    glad_debug_glShadingRateCombinerOpsEXT(combinerOp0, combinerOp1);
	}
#endif // glShadingRateCombinerOpsEXT

#ifdef glShadingRateImageBarrierNV
#undef glShadingRateImageBarrierNV
	static inline void glShadingRateImageBarrierNV(GLboolean synchronize){
	    glad_debug_glShadingRateImageBarrierNV(synchronize);
	}
#endif // glShadingRateImageBarrierNV

#ifdef glShadingRateQCOM
#undef glShadingRateQCOM
	static inline void glShadingRateQCOM(GLenum rate){
	    glad_debug_glShadingRateQCOM(rate);
	}
#endif // glShadingRateQCOM

#ifdef glShadingRateImagePaletteNV
#undef glShadingRateImagePaletteNV
	static inline void glShadingRateImagePaletteNV(GLuint viewport, GLuint first, GLsizei count, const GLenum *rates){
	    glad_debug_glShadingRateImagePaletteNV(viewport, first, count, rates);
	}
#endif // glShadingRateImagePaletteNV

#ifdef glShadingRateSampleOrderNV
#undef glShadingRateSampleOrderNV
	static inline void glShadingRateSampleOrderNV(GLenum order){
	    glad_debug_glShadingRateSampleOrderNV(order);
	}
#endif // glShadingRateSampleOrderNV

#ifdef glShadingRateSampleOrderCustomNV
#undef glShadingRateSampleOrderCustomNV
	static inline void glShadingRateSampleOrderCustomNV(GLenum rate, GLuint samples, const GLint *locations){
	    glad_debug_glShadingRateSampleOrderCustomNV(rate, samples, locations);
	}
#endif // glShadingRateSampleOrderCustomNV

#ifdef glSharpenTexFuncSGIS
#undef glSharpenTexFuncSGIS
	static inline void glSharpenTexFuncSGIS(GLenum target, GLsizei n, const GLfloat *points){
	    glad_debug_glSharpenTexFuncSGIS(target, n, points);
	}
#endif // glSharpenTexFuncSGIS

#ifdef glSignalSemaphoreEXT
#undef glSignalSemaphoreEXT
	static inline void glSignalSemaphoreEXT(GLuint semaphore, GLuint numBufferBarriers, const GLuint *buffers, GLuint numTextureBarriers, const GLuint *textures, const GLenum *dstLayouts){
	    glad_debug_glSignalSemaphoreEXT(semaphore, numBufferBarriers, buffers, numTextureBarriers, textures, dstLayouts);
	}
#endif // glSignalSemaphoreEXT

#ifdef glSignalSemaphoreui64NVX
#undef glSignalSemaphoreui64NVX
	static inline void glSignalSemaphoreui64NVX(GLuint signalGpu, GLsizei fenceObjectCount, const GLuint *semaphoreArray, const GLuint64 *fenceValueArray){
	    glad_debug_glSignalSemaphoreui64NVX(signalGpu, fenceObjectCount, semaphoreArray, fenceValueArray);
	}
#endif // glSignalSemaphoreui64NVX

#ifdef glSpecializeShader
#undef glSpecializeShader
	static inline void glSpecializeShader(GLuint shader, const GLchar *pEntryPoint, GLuint numSpecializationConstants, const GLuint *pConstantIndex, const GLuint *pConstantValue){
	    glad_debug_glSpecializeShader(shader, pEntryPoint, numSpecializationConstants, pConstantIndex, pConstantValue);
	}
#endif // glSpecializeShader

#ifdef glSpecializeShaderARB
#undef glSpecializeShaderARB
	static inline void glSpecializeShaderARB(GLuint shader, const GLchar *pEntryPoint, GLuint numSpecializationConstants, const GLuint *pConstantIndex, const GLuint *pConstantValue){
	    glad_debug_glSpecializeShaderARB(shader, pEntryPoint, numSpecializationConstants, pConstantIndex, pConstantValue);
	}
#endif // glSpecializeShaderARB

#ifdef glSpriteParameterfSGIX
#undef glSpriteParameterfSGIX
	static inline void glSpriteParameterfSGIX(GLenum pname, GLfloat param){
	    glad_debug_glSpriteParameterfSGIX(pname, param);
	}
#endif // glSpriteParameterfSGIX

#ifdef glSpriteParameterfvSGIX
#undef glSpriteParameterfvSGIX
	static inline void glSpriteParameterfvSGIX(GLenum pname, const GLfloat *params){
	    glad_debug_glSpriteParameterfvSGIX(pname, params);
	}
#endif // glSpriteParameterfvSGIX

#ifdef glSpriteParameteriSGIX
#undef glSpriteParameteriSGIX
	static inline void glSpriteParameteriSGIX(GLenum pname, GLint param){
	    glad_debug_glSpriteParameteriSGIX(pname, param);
	}
#endif // glSpriteParameteriSGIX

#ifdef glSpriteParameterivSGIX
#undef glSpriteParameterivSGIX
	static inline void glSpriteParameterivSGIX(GLenum pname, const GLint *params){
	    glad_debug_glSpriteParameterivSGIX(pname, params);
	}
#endif // glSpriteParameterivSGIX

#ifdef glStartInstrumentsSGIX
#undef glStartInstrumentsSGIX
	static inline void glStartInstrumentsSGIX(){
	    glad_debug_glStartInstrumentsSGIX();
	}
#endif // glStartInstrumentsSGIX

#ifdef glStartTilingQCOM
#undef glStartTilingQCOM
	static inline void glStartTilingQCOM(GLuint x, GLuint y, GLuint width, GLuint height, GLbitfield preserveMask){
	    glad_debug_glStartTilingQCOM(x, y, width, height, preserveMask);
	}
#endif // glStartTilingQCOM

#ifdef glStateCaptureNV
#undef glStateCaptureNV
	static inline void glStateCaptureNV(GLuint state, GLenum mode){
	    glad_debug_glStateCaptureNV(state, mode);
	}
#endif // glStateCaptureNV

#ifdef glStencilClearTagEXT
#undef glStencilClearTagEXT
	static inline void glStencilClearTagEXT(GLsizei stencilTagBits, GLuint stencilClearTag){
	    glad_debug_glStencilClearTagEXT(stencilTagBits, stencilClearTag);
	}
#endif // glStencilClearTagEXT

#ifdef glStencilFillPathNV
#undef glStencilFillPathNV
	static inline void glStencilFillPathNV(GLuint path, GLenum fillMode, GLuint mask){
	    glad_debug_glStencilFillPathNV(path, fillMode, mask);
	}
#endif // glStencilFillPathNV

#ifdef glStencilFunc
#undef glStencilFunc
	static inline void glStencilFunc(GLenum func, GLint ref, GLuint mask){
	    glad_debug_glStencilFunc(func, ref, mask);
	}
#endif // glStencilFunc

#ifdef glStencilFuncSeparate
#undef glStencilFuncSeparate
	static inline void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask){
	    glad_debug_glStencilFuncSeparate(face, func, ref, mask);
	}
#endif // glStencilFuncSeparate

#ifdef glStencilFuncSeparateATI
#undef glStencilFuncSeparateATI
	static inline void glStencilFuncSeparateATI(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask){
	    glad_debug_glStencilFuncSeparateATI(frontfunc, backfunc, ref, mask);
	}
#endif // glStencilFuncSeparateATI

#ifdef glStencilMask
#undef glStencilMask
	static inline void glStencilMask(GLuint mask){
	    glad_debug_glStencilMask(mask);
	}
#endif // glStencilMask

#ifdef glStencilMaskSeparate
#undef glStencilMaskSeparate
	static inline void glStencilMaskSeparate(GLenum face, GLuint mask){
	    glad_debug_glStencilMaskSeparate(face, mask);
	}
#endif // glStencilMaskSeparate

#ifdef glStencilOp
#undef glStencilOp
	static inline void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass){
	    glad_debug_glStencilOp(fail, zfail, zpass);
	}
#endif // glStencilOp

#ifdef glStencilOpSeparate
#undef glStencilOpSeparate
	static inline void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass){
	    glad_debug_glStencilOpSeparate(face, sfail, dpfail, dppass);
	}
#endif // glStencilOpSeparate

#ifdef glStencilOpSeparateATI
#undef glStencilOpSeparateATI
	static inline void glStencilOpSeparateATI(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass){
	    glad_debug_glStencilOpSeparateATI(face, sfail, dpfail, dppass);
	}
#endif // glStencilOpSeparateATI

#ifdef glStencilOpValueAMD
#undef glStencilOpValueAMD
	static inline void glStencilOpValueAMD(GLenum face, GLuint value){
	    glad_debug_glStencilOpValueAMD(face, value);
	}
#endif // glStencilOpValueAMD

#ifdef glStencilStrokePathNV
#undef glStencilStrokePathNV
	static inline void glStencilStrokePathNV(GLuint path, GLint reference, GLuint mask){
	    glad_debug_glStencilStrokePathNV(path, reference, mask);
	}
#endif // glStencilStrokePathNV

#ifdef glStencilThenCoverFillPathNV
#undef glStencilThenCoverFillPathNV
	static inline void glStencilThenCoverFillPathNV(GLuint path, GLenum fillMode, GLuint mask, GLenum coverMode){
	    glad_debug_glStencilThenCoverFillPathNV(path, fillMode, mask, coverMode);
	}
#endif // glStencilThenCoverFillPathNV

#ifdef glStencilThenCoverStrokePathNV
#undef glStencilThenCoverStrokePathNV
	static inline void glStencilThenCoverStrokePathNV(GLuint path, GLint reference, GLuint mask, GLenum coverMode){
	    glad_debug_glStencilThenCoverStrokePathNV(path, reference, mask, coverMode);
	}
#endif // glStencilThenCoverStrokePathNV

#ifdef glStopInstrumentsSGIX
#undef glStopInstrumentsSGIX
	static inline void glStopInstrumentsSGIX(GLint marker){
	    glad_debug_glStopInstrumentsSGIX(marker);
	}
#endif // glStopInstrumentsSGIX

#ifdef glSubpixelPrecisionBiasNV
#undef glSubpixelPrecisionBiasNV
	static inline void glSubpixelPrecisionBiasNV(GLuint xbits, GLuint ybits){
	    glad_debug_glSubpixelPrecisionBiasNV(xbits, ybits);
	}
#endif // glSubpixelPrecisionBiasNV

#ifdef glSwizzleEXT
#undef glSwizzleEXT
	static inline void glSwizzleEXT(GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW){
	    glad_debug_glSwizzleEXT(res, in, outX, outY, outZ, outW);
	}
#endif // glSwizzleEXT

#ifdef glSyncTextureINTEL
#undef glSyncTextureINTEL
	static inline void glSyncTextureINTEL(GLuint texture){
	    glad_debug_glSyncTextureINTEL(texture);
	}
#endif // glSyncTextureINTEL

#ifdef glTagSampleBufferSGIX
#undef glTagSampleBufferSGIX
	static inline void glTagSampleBufferSGIX(){
	    glad_debug_glTagSampleBufferSGIX();
	}
#endif // glTagSampleBufferSGIX

#ifdef glTangent3bEXT
#undef glTangent3bEXT
	static inline void glTangent3bEXT(GLbyte tx, GLbyte ty, GLbyte tz){
	    glad_debug_glTangent3bEXT(tx, ty, tz);
	}
#endif // glTangent3bEXT

#ifdef glTangent3bvEXT
#undef glTangent3bvEXT
	static inline void glTangent3bvEXT(const GLbyte *v){
	    glad_debug_glTangent3bvEXT(v);
	}
#endif // glTangent3bvEXT

#ifdef glTangent3dEXT
#undef glTangent3dEXT
	static inline void glTangent3dEXT(GLdouble tx, GLdouble ty, GLdouble tz){
	    glad_debug_glTangent3dEXT(tx, ty, tz);
	}
#endif // glTangent3dEXT

#ifdef glTangent3dvEXT
#undef glTangent3dvEXT
	static inline void glTangent3dvEXT(const GLdouble *v){
	    glad_debug_glTangent3dvEXT(v);
	}
#endif // glTangent3dvEXT

#ifdef glTangent3fEXT
#undef glTangent3fEXT
	static inline void glTangent3fEXT(GLfloat tx, GLfloat ty, GLfloat tz){
	    glad_debug_glTangent3fEXT(tx, ty, tz);
	}
#endif // glTangent3fEXT

#ifdef glTangent3fvEXT
#undef glTangent3fvEXT
	static inline void glTangent3fvEXT(const GLfloat *v){
	    glad_debug_glTangent3fvEXT(v);
	}
#endif // glTangent3fvEXT

#ifdef glTangent3iEXT
#undef glTangent3iEXT
	static inline void glTangent3iEXT(GLint tx, GLint ty, GLint tz){
	    glad_debug_glTangent3iEXT(tx, ty, tz);
	}
#endif // glTangent3iEXT

#ifdef glTangent3ivEXT
#undef glTangent3ivEXT
	static inline void glTangent3ivEXT(const GLint *v){
	    glad_debug_glTangent3ivEXT(v);
	}
#endif // glTangent3ivEXT

#ifdef glTangent3sEXT
#undef glTangent3sEXT
	static inline void glTangent3sEXT(GLshort tx, GLshort ty, GLshort tz){
	    glad_debug_glTangent3sEXT(tx, ty, tz);
	}
#endif // glTangent3sEXT

#ifdef glTangent3svEXT
#undef glTangent3svEXT
	static inline void glTangent3svEXT(const GLshort *v){
	    glad_debug_glTangent3svEXT(v);
	}
#endif // glTangent3svEXT

#ifdef glTbufferMask3DFX
#undef glTbufferMask3DFX
	static inline void glTbufferMask3DFX(GLuint mask){
	    glad_debug_glTbufferMask3DFX(mask);
	}
#endif // glTbufferMask3DFX

#ifdef glTessellationFactorAMD
#undef glTessellationFactorAMD
	static inline void glTessellationFactorAMD(GLfloat factor){
	    glad_debug_glTessellationFactorAMD(factor);
	}
#endif // glTessellationFactorAMD

#ifdef glTessellationModeAMD
#undef glTessellationModeAMD
	static inline void glTessellationModeAMD(GLenum mode){
	    glad_debug_glTessellationModeAMD(mode);
	}
#endif // glTessellationModeAMD

#ifdef glTexAttachMemoryNV
#undef glTexAttachMemoryNV
	static inline void glTexAttachMemoryNV(GLenum target, GLuint memory, GLuint64 offset){
	    glad_debug_glTexAttachMemoryNV(target, memory, offset);
	}
#endif // glTexAttachMemoryNV

#ifdef glTexBuffer
#undef glTexBuffer
	static inline void glTexBuffer(GLenum target, GLenum internalformat, GLuint buffer){
	    glad_debug_glTexBuffer(target, internalformat, buffer);
	}
#endif // glTexBuffer

#ifdef glTexBufferARB
#undef glTexBufferARB
	static inline void glTexBufferARB(GLenum target, GLenum internalformat, GLuint buffer){
	    glad_debug_glTexBufferARB(target, internalformat, buffer);
	}
#endif // glTexBufferARB

#ifdef glTexBufferEXT
#undef glTexBufferEXT
	static inline void glTexBufferEXT(GLenum target, GLenum internalformat, GLuint buffer){
	    glad_debug_glTexBufferEXT(target, internalformat, buffer);
	}
#endif // glTexBufferEXT

#ifdef glTexBufferOES
#undef glTexBufferOES
	static inline void glTexBufferOES(GLenum target, GLenum internalformat, GLuint buffer){
	    glad_debug_glTexBufferOES(target, internalformat, buffer);
	}
#endif // glTexBufferOES

#ifdef glTexBufferRange
#undef glTexBufferRange
	static inline void glTexBufferRange(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size){
	    glad_debug_glTexBufferRange(target, internalformat, buffer, offset, size);
	}
#endif // glTexBufferRange

#ifdef glTexBufferRangeEXT
#undef glTexBufferRangeEXT
	static inline void glTexBufferRangeEXT(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size){
	    glad_debug_glTexBufferRangeEXT(target, internalformat, buffer, offset, size);
	}
#endif // glTexBufferRangeEXT

#ifdef glTexBufferRangeOES
#undef glTexBufferRangeOES
	static inline void glTexBufferRangeOES(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size){
	    glad_debug_glTexBufferRangeOES(target, internalformat, buffer, offset, size);
	}
#endif // glTexBufferRangeOES

#ifdef glTexBumpParameterfvATI
#undef glTexBumpParameterfvATI
	static inline void glTexBumpParameterfvATI(GLenum pname, const GLfloat *param){
	    glad_debug_glTexBumpParameterfvATI(pname, param);
	}
#endif // glTexBumpParameterfvATI

#ifdef glTexBumpParameterivATI
#undef glTexBumpParameterivATI
	static inline void glTexBumpParameterivATI(GLenum pname, const GLint *param){
	    glad_debug_glTexBumpParameterivATI(pname, param);
	}
#endif // glTexBumpParameterivATI

#ifdef glTexCoord1bOES
#undef glTexCoord1bOES
	static inline void glTexCoord1bOES(GLbyte s){
	    glad_debug_glTexCoord1bOES(s);
	}
#endif // glTexCoord1bOES

#ifdef glTexCoord1bvOES
#undef glTexCoord1bvOES
	static inline void glTexCoord1bvOES(const GLbyte *coords){
	    glad_debug_glTexCoord1bvOES(coords);
	}
#endif // glTexCoord1bvOES

#ifdef glTexCoord1d
#undef glTexCoord1d
	static inline void glTexCoord1d(GLdouble s){
	    glad_debug_glTexCoord1d(s);
	}
#endif // glTexCoord1d

#ifdef glTexCoord1dv
#undef glTexCoord1dv
	static inline void glTexCoord1dv(const GLdouble *v){
	    glad_debug_glTexCoord1dv(v);
	}
#endif // glTexCoord1dv

#ifdef glTexCoord1f
#undef glTexCoord1f
	static inline void glTexCoord1f(GLfloat s){
	    glad_debug_glTexCoord1f(s);
	}
#endif // glTexCoord1f

#ifdef glTexCoord1fv
#undef glTexCoord1fv
	static inline void glTexCoord1fv(const GLfloat *v){
	    glad_debug_glTexCoord1fv(v);
	}
#endif // glTexCoord1fv

#ifdef glTexCoord1hNV
#undef glTexCoord1hNV
	static inline void glTexCoord1hNV(GLhalfNV s){
	    glad_debug_glTexCoord1hNV(s);
	}
#endif // glTexCoord1hNV

#ifdef glTexCoord1hvNV
#undef glTexCoord1hvNV
	static inline void glTexCoord1hvNV(const GLhalfNV *v){
	    glad_debug_glTexCoord1hvNV(v);
	}
#endif // glTexCoord1hvNV

#ifdef glTexCoord1i
#undef glTexCoord1i
	static inline void glTexCoord1i(GLint s){
	    glad_debug_glTexCoord1i(s);
	}
#endif // glTexCoord1i

#ifdef glTexCoord1iv
#undef glTexCoord1iv
	static inline void glTexCoord1iv(const GLint *v){
	    glad_debug_glTexCoord1iv(v);
	}
#endif // glTexCoord1iv

#ifdef glTexCoord1s
#undef glTexCoord1s
	static inline void glTexCoord1s(GLshort s){
	    glad_debug_glTexCoord1s(s);
	}
#endif // glTexCoord1s

#ifdef glTexCoord1sv
#undef glTexCoord1sv
	static inline void glTexCoord1sv(const GLshort *v){
	    glad_debug_glTexCoord1sv(v);
	}
#endif // glTexCoord1sv

#ifdef glTexCoord1xOES
#undef glTexCoord1xOES
	static inline void glTexCoord1xOES(GLfixed s){
	    glad_debug_glTexCoord1xOES(s);
	}
#endif // glTexCoord1xOES

#ifdef glTexCoord1xvOES
#undef glTexCoord1xvOES
	static inline void glTexCoord1xvOES(const GLfixed *coords){
	    glad_debug_glTexCoord1xvOES(coords);
	}
#endif // glTexCoord1xvOES

#ifdef glTexCoord2bOES
#undef glTexCoord2bOES
	static inline void glTexCoord2bOES(GLbyte s, GLbyte t){
	    glad_debug_glTexCoord2bOES(s, t);
	}
#endif // glTexCoord2bOES

#ifdef glTexCoord2bvOES
#undef glTexCoord2bvOES
	static inline void glTexCoord2bvOES(const GLbyte *coords){
	    glad_debug_glTexCoord2bvOES(coords);
	}
#endif // glTexCoord2bvOES

#ifdef glTexCoord2d
#undef glTexCoord2d
	static inline void glTexCoord2d(GLdouble s, GLdouble t){
	    glad_debug_glTexCoord2d(s, t);
	}
#endif // glTexCoord2d

#ifdef glTexCoord2dv
#undef glTexCoord2dv
	static inline void glTexCoord2dv(const GLdouble *v){
	    glad_debug_glTexCoord2dv(v);
	}
#endif // glTexCoord2dv

#ifdef glTexCoord2f
#undef glTexCoord2f
	static inline void glTexCoord2f(GLfloat s, GLfloat t){
	    glad_debug_glTexCoord2f(s, t);
	}
#endif // glTexCoord2f

#ifdef glTexCoord2fColor3fVertex3fSUN
#undef glTexCoord2fColor3fVertex3fSUN
	static inline void glTexCoord2fColor3fVertex3fSUN(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glTexCoord2fColor3fVertex3fSUN(s, t, r, g, b, x, y, z);
	}
#endif // glTexCoord2fColor3fVertex3fSUN

#ifdef glTexCoord2fColor3fVertex3fvSUN
#undef glTexCoord2fColor3fVertex3fvSUN
	static inline void glTexCoord2fColor3fVertex3fvSUN(const GLfloat *tc, const GLfloat *c, const GLfloat *v){
	    glad_debug_glTexCoord2fColor3fVertex3fvSUN(tc, c, v);
	}
#endif // glTexCoord2fColor3fVertex3fvSUN

#ifdef glTexCoord2fColor4fNormal3fVertex3fSUN
#undef glTexCoord2fColor4fNormal3fVertex3fSUN
	static inline void glTexCoord2fColor4fNormal3fVertex3fSUN(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glTexCoord2fColor4fNormal3fVertex3fSUN(s, t, r, g, b, a, nx, ny, nz, x, y, z);
	}
#endif // glTexCoord2fColor4fNormal3fVertex3fSUN

#ifdef glTexCoord2fColor4fNormal3fVertex3fvSUN
#undef glTexCoord2fColor4fNormal3fVertex3fvSUN
	static inline void glTexCoord2fColor4fNormal3fVertex3fvSUN(const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v){
	    glad_debug_glTexCoord2fColor4fNormal3fVertex3fvSUN(tc, c, n, v);
	}
#endif // glTexCoord2fColor4fNormal3fVertex3fvSUN

#ifdef glTexCoord2fColor4ubVertex3fSUN
#undef glTexCoord2fColor4ubVertex3fSUN
	static inline void glTexCoord2fColor4ubVertex3fSUN(GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glTexCoord2fColor4ubVertex3fSUN(s, t, r, g, b, a, x, y, z);
	}
#endif // glTexCoord2fColor4ubVertex3fSUN

#ifdef glTexCoord2fColor4ubVertex3fvSUN
#undef glTexCoord2fColor4ubVertex3fvSUN
	static inline void glTexCoord2fColor4ubVertex3fvSUN(const GLfloat *tc, const GLubyte *c, const GLfloat *v){
	    glad_debug_glTexCoord2fColor4ubVertex3fvSUN(tc, c, v);
	}
#endif // glTexCoord2fColor4ubVertex3fvSUN

#ifdef glTexCoord2fNormal3fVertex3fSUN
#undef glTexCoord2fNormal3fVertex3fSUN
	static inline void glTexCoord2fNormal3fVertex3fSUN(GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glTexCoord2fNormal3fVertex3fSUN(s, t, nx, ny, nz, x, y, z);
	}
#endif // glTexCoord2fNormal3fVertex3fSUN

#ifdef glTexCoord2fNormal3fVertex3fvSUN
#undef glTexCoord2fNormal3fVertex3fvSUN
	static inline void glTexCoord2fNormal3fVertex3fvSUN(const GLfloat *tc, const GLfloat *n, const GLfloat *v){
	    glad_debug_glTexCoord2fNormal3fVertex3fvSUN(tc, n, v);
	}
#endif // glTexCoord2fNormal3fVertex3fvSUN

#ifdef glTexCoord2fVertex3fSUN
#undef glTexCoord2fVertex3fSUN
	static inline void glTexCoord2fVertex3fSUN(GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glTexCoord2fVertex3fSUN(s, t, x, y, z);
	}
#endif // glTexCoord2fVertex3fSUN

#ifdef glTexCoord2fVertex3fvSUN
#undef glTexCoord2fVertex3fvSUN
	static inline void glTexCoord2fVertex3fvSUN(const GLfloat *tc, const GLfloat *v){
	    glad_debug_glTexCoord2fVertex3fvSUN(tc, v);
	}
#endif // glTexCoord2fVertex3fvSUN

#ifdef glTexCoord2fv
#undef glTexCoord2fv
	static inline void glTexCoord2fv(const GLfloat *v){
	    glad_debug_glTexCoord2fv(v);
	}
#endif // glTexCoord2fv

#ifdef glTexCoord2hNV
#undef glTexCoord2hNV
	static inline void glTexCoord2hNV(GLhalfNV s, GLhalfNV t){
	    glad_debug_glTexCoord2hNV(s, t);
	}
#endif // glTexCoord2hNV

#ifdef glTexCoord2hvNV
#undef glTexCoord2hvNV
	static inline void glTexCoord2hvNV(const GLhalfNV *v){
	    glad_debug_glTexCoord2hvNV(v);
	}
#endif // glTexCoord2hvNV

#ifdef glTexCoord2i
#undef glTexCoord2i
	static inline void glTexCoord2i(GLint s, GLint t){
	    glad_debug_glTexCoord2i(s, t);
	}
#endif // glTexCoord2i

#ifdef glTexCoord2iv
#undef glTexCoord2iv
	static inline void glTexCoord2iv(const GLint *v){
	    glad_debug_glTexCoord2iv(v);
	}
#endif // glTexCoord2iv

#ifdef glTexCoord2s
#undef glTexCoord2s
	static inline void glTexCoord2s(GLshort s, GLshort t){
	    glad_debug_glTexCoord2s(s, t);
	}
#endif // glTexCoord2s

#ifdef glTexCoord2sv
#undef glTexCoord2sv
	static inline void glTexCoord2sv(const GLshort *v){
	    glad_debug_glTexCoord2sv(v);
	}
#endif // glTexCoord2sv

#ifdef glTexCoord2xOES
#undef glTexCoord2xOES
	static inline void glTexCoord2xOES(GLfixed s, GLfixed t){
	    glad_debug_glTexCoord2xOES(s, t);
	}
#endif // glTexCoord2xOES

#ifdef glTexCoord2xvOES
#undef glTexCoord2xvOES
	static inline void glTexCoord2xvOES(const GLfixed *coords){
	    glad_debug_glTexCoord2xvOES(coords);
	}
#endif // glTexCoord2xvOES

#ifdef glTexCoord3bOES
#undef glTexCoord3bOES
	static inline void glTexCoord3bOES(GLbyte s, GLbyte t, GLbyte r){
	    glad_debug_glTexCoord3bOES(s, t, r);
	}
#endif // glTexCoord3bOES

#ifdef glTexCoord3bvOES
#undef glTexCoord3bvOES
	static inline void glTexCoord3bvOES(const GLbyte *coords){
	    glad_debug_glTexCoord3bvOES(coords);
	}
#endif // glTexCoord3bvOES

#ifdef glTexCoord3d
#undef glTexCoord3d
	static inline void glTexCoord3d(GLdouble s, GLdouble t, GLdouble r){
	    glad_debug_glTexCoord3d(s, t, r);
	}
#endif // glTexCoord3d

#ifdef glTexCoord3dv
#undef glTexCoord3dv
	static inline void glTexCoord3dv(const GLdouble *v){
	    glad_debug_glTexCoord3dv(v);
	}
#endif // glTexCoord3dv

#ifdef glTexCoord3f
#undef glTexCoord3f
	static inline void glTexCoord3f(GLfloat s, GLfloat t, GLfloat r){
	    glad_debug_glTexCoord3f(s, t, r);
	}
#endif // glTexCoord3f

#ifdef glTexCoord3fv
#undef glTexCoord3fv
	static inline void glTexCoord3fv(const GLfloat *v){
	    glad_debug_glTexCoord3fv(v);
	}
#endif // glTexCoord3fv

#ifdef glTexCoord3hNV
#undef glTexCoord3hNV
	static inline void glTexCoord3hNV(GLhalfNV s, GLhalfNV t, GLhalfNV r){
	    glad_debug_glTexCoord3hNV(s, t, r);
	}
#endif // glTexCoord3hNV

#ifdef glTexCoord3hvNV
#undef glTexCoord3hvNV
	static inline void glTexCoord3hvNV(const GLhalfNV *v){
	    glad_debug_glTexCoord3hvNV(v);
	}
#endif // glTexCoord3hvNV

#ifdef glTexCoord3i
#undef glTexCoord3i
	static inline void glTexCoord3i(GLint s, GLint t, GLint r){
	    glad_debug_glTexCoord3i(s, t, r);
	}
#endif // glTexCoord3i

#ifdef glTexCoord3iv
#undef glTexCoord3iv
	static inline void glTexCoord3iv(const GLint *v){
	    glad_debug_glTexCoord3iv(v);
	}
#endif // glTexCoord3iv

#ifdef glTexCoord3s
#undef glTexCoord3s
	static inline void glTexCoord3s(GLshort s, GLshort t, GLshort r){
	    glad_debug_glTexCoord3s(s, t, r);
	}
#endif // glTexCoord3s

#ifdef glTexCoord3sv
#undef glTexCoord3sv
	static inline void glTexCoord3sv(const GLshort *v){
	    glad_debug_glTexCoord3sv(v);
	}
#endif // glTexCoord3sv

#ifdef glTexCoord3xOES
#undef glTexCoord3xOES
	static inline void glTexCoord3xOES(GLfixed s, GLfixed t, GLfixed r){
	    glad_debug_glTexCoord3xOES(s, t, r);
	}
#endif // glTexCoord3xOES

#ifdef glTexCoord3xvOES
#undef glTexCoord3xvOES
	static inline void glTexCoord3xvOES(const GLfixed *coords){
	    glad_debug_glTexCoord3xvOES(coords);
	}
#endif // glTexCoord3xvOES

#ifdef glTexCoord4bOES
#undef glTexCoord4bOES
	static inline void glTexCoord4bOES(GLbyte s, GLbyte t, GLbyte r, GLbyte q){
	    glad_debug_glTexCoord4bOES(s, t, r, q);
	}
#endif // glTexCoord4bOES

#ifdef glTexCoord4bvOES
#undef glTexCoord4bvOES
	static inline void glTexCoord4bvOES(const GLbyte *coords){
	    glad_debug_glTexCoord4bvOES(coords);
	}
#endif // glTexCoord4bvOES

#ifdef glTexCoord4d
#undef glTexCoord4d
	static inline void glTexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q){
	    glad_debug_glTexCoord4d(s, t, r, q);
	}
#endif // glTexCoord4d

#ifdef glTexCoord4dv
#undef glTexCoord4dv
	static inline void glTexCoord4dv(const GLdouble *v){
	    glad_debug_glTexCoord4dv(v);
	}
#endif // glTexCoord4dv

#ifdef glTexCoord4f
#undef glTexCoord4f
	static inline void glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q){
	    glad_debug_glTexCoord4f(s, t, r, q);
	}
#endif // glTexCoord4f

#ifdef glTexCoord4fColor4fNormal3fVertex4fSUN
#undef glTexCoord4fColor4fNormal3fVertex4fSUN
	static inline void glTexCoord4fColor4fNormal3fVertex4fSUN(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glTexCoord4fColor4fNormal3fVertex4fSUN(s, t, p, q, r, g, b, a, nx, ny, nz, x, y, z, w);
	}
#endif // glTexCoord4fColor4fNormal3fVertex4fSUN

#ifdef glTexCoord4fColor4fNormal3fVertex4fvSUN
#undef glTexCoord4fColor4fNormal3fVertex4fvSUN
	static inline void glTexCoord4fColor4fNormal3fVertex4fvSUN(const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v){
	    glad_debug_glTexCoord4fColor4fNormal3fVertex4fvSUN(tc, c, n, v);
	}
#endif // glTexCoord4fColor4fNormal3fVertex4fvSUN

#ifdef glTexCoord4fVertex4fSUN
#undef glTexCoord4fVertex4fSUN
	static inline void glTexCoord4fVertex4fSUN(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glTexCoord4fVertex4fSUN(s, t, p, q, x, y, z, w);
	}
#endif // glTexCoord4fVertex4fSUN

#ifdef glTexCoord4fVertex4fvSUN
#undef glTexCoord4fVertex4fvSUN
	static inline void glTexCoord4fVertex4fvSUN(const GLfloat *tc, const GLfloat *v){
	    glad_debug_glTexCoord4fVertex4fvSUN(tc, v);
	}
#endif // glTexCoord4fVertex4fvSUN

#ifdef glTexCoord4fv
#undef glTexCoord4fv
	static inline void glTexCoord4fv(const GLfloat *v){
	    glad_debug_glTexCoord4fv(v);
	}
#endif // glTexCoord4fv

#ifdef glTexCoord4hNV
#undef glTexCoord4hNV
	static inline void glTexCoord4hNV(GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q){
	    glad_debug_glTexCoord4hNV(s, t, r, q);
	}
#endif // glTexCoord4hNV

#ifdef glTexCoord4hvNV
#undef glTexCoord4hvNV
	static inline void glTexCoord4hvNV(const GLhalfNV *v){
	    glad_debug_glTexCoord4hvNV(v);
	}
#endif // glTexCoord4hvNV

#ifdef glTexCoord4i
#undef glTexCoord4i
	static inline void glTexCoord4i(GLint s, GLint t, GLint r, GLint q){
	    glad_debug_glTexCoord4i(s, t, r, q);
	}
#endif // glTexCoord4i

#ifdef glTexCoord4iv
#undef glTexCoord4iv
	static inline void glTexCoord4iv(const GLint *v){
	    glad_debug_glTexCoord4iv(v);
	}
#endif // glTexCoord4iv

#ifdef glTexCoord4s
#undef glTexCoord4s
	static inline void glTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q){
	    glad_debug_glTexCoord4s(s, t, r, q);
	}
#endif // glTexCoord4s

#ifdef glTexCoord4sv
#undef glTexCoord4sv
	static inline void glTexCoord4sv(const GLshort *v){
	    glad_debug_glTexCoord4sv(v);
	}
#endif // glTexCoord4sv

#ifdef glTexCoord4xOES
#undef glTexCoord4xOES
	static inline void glTexCoord4xOES(GLfixed s, GLfixed t, GLfixed r, GLfixed q){
	    glad_debug_glTexCoord4xOES(s, t, r, q);
	}
#endif // glTexCoord4xOES

#ifdef glTexCoord4xvOES
#undef glTexCoord4xvOES
	static inline void glTexCoord4xvOES(const GLfixed *coords){
	    glad_debug_glTexCoord4xvOES(coords);
	}
#endif // glTexCoord4xvOES

#ifdef glTexCoordFormatNV
#undef glTexCoordFormatNV
	static inline void glTexCoordFormatNV(GLint size, GLenum type, GLsizei stride){
	    glad_debug_glTexCoordFormatNV(size, type, stride);
	}
#endif // glTexCoordFormatNV

#ifdef glTexCoordP1ui
#undef glTexCoordP1ui
	static inline void glTexCoordP1ui(GLenum type, GLuint coords){
	    glad_debug_glTexCoordP1ui(type, coords);
	}
#endif // glTexCoordP1ui

#ifdef glTexCoordP1uiv
#undef glTexCoordP1uiv
	static inline void glTexCoordP1uiv(GLenum type, const GLuint *coords){
	    glad_debug_glTexCoordP1uiv(type, coords);
	}
#endif // glTexCoordP1uiv

#ifdef glTexCoordP2ui
#undef glTexCoordP2ui
	static inline void glTexCoordP2ui(GLenum type, GLuint coords){
	    glad_debug_glTexCoordP2ui(type, coords);
	}
#endif // glTexCoordP2ui

#ifdef glTexCoordP2uiv
#undef glTexCoordP2uiv
	static inline void glTexCoordP2uiv(GLenum type, const GLuint *coords){
	    glad_debug_glTexCoordP2uiv(type, coords);
	}
#endif // glTexCoordP2uiv

#ifdef glTexCoordP3ui
#undef glTexCoordP3ui
	static inline void glTexCoordP3ui(GLenum type, GLuint coords){
	    glad_debug_glTexCoordP3ui(type, coords);
	}
#endif // glTexCoordP3ui

#ifdef glTexCoordP3uiv
#undef glTexCoordP3uiv
	static inline void glTexCoordP3uiv(GLenum type, const GLuint *coords){
	    glad_debug_glTexCoordP3uiv(type, coords);
	}
#endif // glTexCoordP3uiv

#ifdef glTexCoordP4ui
#undef glTexCoordP4ui
	static inline void glTexCoordP4ui(GLenum type, GLuint coords){
	    glad_debug_glTexCoordP4ui(type, coords);
	}
#endif // glTexCoordP4ui

#ifdef glTexCoordP4uiv
#undef glTexCoordP4uiv
	static inline void glTexCoordP4uiv(GLenum type, const GLuint *coords){
	    glad_debug_glTexCoordP4uiv(type, coords);
	}
#endif // glTexCoordP4uiv

#ifdef glTexEnvf
#undef glTexEnvf
	static inline void glTexEnvf(GLenum target, GLenum pname, GLfloat param){
	    glad_debug_glTexEnvf(target, pname, param);
	}
#endif // glTexEnvf

#ifdef glTexEnvfv
#undef glTexEnvfv
	static inline void glTexEnvfv(GLenum target, GLenum pname, const GLfloat *params){
	    glad_debug_glTexEnvfv(target, pname, params);
	}
#endif // glTexEnvfv

#ifdef glTexEnvi
#undef glTexEnvi
	static inline void glTexEnvi(GLenum target, GLenum pname, GLint param){
	    glad_debug_glTexEnvi(target, pname, param);
	}
#endif // glTexEnvi

#ifdef glTexEnviv
#undef glTexEnviv
	static inline void glTexEnviv(GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glTexEnviv(target, pname, params);
	}
#endif // glTexEnviv

#ifdef glTexEnvx
#undef glTexEnvx
	static inline void glTexEnvx(GLenum target, GLenum pname, GLfixed param){
	    glad_debug_glTexEnvx(target, pname, param);
	}
#endif // glTexEnvx

#ifdef glTexEnvxOES
#undef glTexEnvxOES
	static inline void glTexEnvxOES(GLenum target, GLenum pname, GLfixed param){
	    glad_debug_glTexEnvxOES(target, pname, param);
	}
#endif // glTexEnvxOES

#ifdef glTexEnvxv
#undef glTexEnvxv
	static inline void glTexEnvxv(GLenum target, GLenum pname, const GLfixed *params){
	    glad_debug_glTexEnvxv(target, pname, params);
	}
#endif // glTexEnvxv

#ifdef glTexEnvxvOES
#undef glTexEnvxvOES
	static inline void glTexEnvxvOES(GLenum target, GLenum pname, const GLfixed *params){
	    glad_debug_glTexEnvxvOES(target, pname, params);
	}
#endif // glTexEnvxvOES

#ifdef glTexEstimateMotionQCOM
#undef glTexEstimateMotionQCOM
	static inline void glTexEstimateMotionQCOM(GLuint ref, GLuint target, GLuint output){
	    glad_debug_glTexEstimateMotionQCOM(ref, target, output);
	}
#endif // glTexEstimateMotionQCOM

#ifdef glTexEstimateMotionRegionsQCOM
#undef glTexEstimateMotionRegionsQCOM
	static inline void glTexEstimateMotionRegionsQCOM(GLuint ref, GLuint target, GLuint output, GLuint mask){
	    glad_debug_glTexEstimateMotionRegionsQCOM(ref, target, output, mask);
	}
#endif // glTexEstimateMotionRegionsQCOM

#ifdef glExtrapolateTex2DQCOM
#undef glExtrapolateTex2DQCOM
	static inline void glExtrapolateTex2DQCOM(GLuint src1, GLuint src2, GLuint output, GLfloat scaleFactor){
	    glad_debug_glExtrapolateTex2DQCOM(src1, src2, output, scaleFactor);
	}
#endif // glExtrapolateTex2DQCOM

#ifdef glTexFilterFuncSGIS
#undef glTexFilterFuncSGIS
	static inline void glTexFilterFuncSGIS(GLenum target, GLenum filter, GLsizei n, const GLfloat *weights){
	    glad_debug_glTexFilterFuncSGIS(target, filter, n, weights);
	}
#endif // glTexFilterFuncSGIS

#ifdef glTexGend
#undef glTexGend
	static inline void glTexGend(GLenum coord, GLenum pname, GLdouble param){
	    glad_debug_glTexGend(coord, pname, param);
	}
#endif // glTexGend

#ifdef glTexGendv
#undef glTexGendv
	static inline void glTexGendv(GLenum coord, GLenum pname, const GLdouble *params){
	    glad_debug_glTexGendv(coord, pname, params);
	}
#endif // glTexGendv

#ifdef glTexGenf
#undef glTexGenf
	static inline void glTexGenf(GLenum coord, GLenum pname, GLfloat param){
	    glad_debug_glTexGenf(coord, pname, param);
	}
#endif // glTexGenf

#ifdef glTexGenfOES
#undef glTexGenfOES
	static inline void glTexGenfOES(GLenum coord, GLenum pname, GLfloat param){
	    glad_debug_glTexGenfOES(coord, pname, param);
	}
#endif // glTexGenfOES

#ifdef glTexGenfv
#undef glTexGenfv
	static inline void glTexGenfv(GLenum coord, GLenum pname, const GLfloat *params){
	    glad_debug_glTexGenfv(coord, pname, params);
	}
#endif // glTexGenfv

#ifdef glTexGenfvOES
#undef glTexGenfvOES
	static inline void glTexGenfvOES(GLenum coord, GLenum pname, const GLfloat *params){
	    glad_debug_glTexGenfvOES(coord, pname, params);
	}
#endif // glTexGenfvOES

#ifdef glTexGeni
#undef glTexGeni
	static inline void glTexGeni(GLenum coord, GLenum pname, GLint param){
	    glad_debug_glTexGeni(coord, pname, param);
	}
#endif // glTexGeni

#ifdef glTexGeniOES
#undef glTexGeniOES
	static inline void glTexGeniOES(GLenum coord, GLenum pname, GLint param){
	    glad_debug_glTexGeniOES(coord, pname, param);
	}
#endif // glTexGeniOES

#ifdef glTexGeniv
#undef glTexGeniv
	static inline void glTexGeniv(GLenum coord, GLenum pname, const GLint *params){
	    glad_debug_glTexGeniv(coord, pname, params);
	}
#endif // glTexGeniv

#ifdef glTexGenivOES
#undef glTexGenivOES
	static inline void glTexGenivOES(GLenum coord, GLenum pname, const GLint *params){
	    glad_debug_glTexGenivOES(coord, pname, params);
	}
#endif // glTexGenivOES

#ifdef glTexGenxOES
#undef glTexGenxOES
	static inline void glTexGenxOES(GLenum coord, GLenum pname, GLfixed param){
	    glad_debug_glTexGenxOES(coord, pname, param);
	}
#endif // glTexGenxOES

#ifdef glTexGenxvOES
#undef glTexGenxvOES
	static inline void glTexGenxvOES(GLenum coord, GLenum pname, const GLfixed *params){
	    glad_debug_glTexGenxvOES(coord, pname, params);
	}
#endif // glTexGenxvOES

#ifdef glTexImage2DMultisample
#undef glTexImage2DMultisample
	static inline void glTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations){
	    glad_debug_glTexImage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
	}
#endif // glTexImage2DMultisample

#ifdef glTexImage2DMultisampleCoverageNV
#undef glTexImage2DMultisampleCoverageNV
	static inline void glTexImage2DMultisampleCoverageNV(GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations){
	    glad_debug_glTexImage2DMultisampleCoverageNV(target, coverageSamples, colorSamples, internalFormat, width, height, fixedSampleLocations);
	}
#endif // glTexImage2DMultisampleCoverageNV

#ifdef glTexImage3DMultisample
#undef glTexImage3DMultisample
	static inline void glTexImage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations){
	    glad_debug_glTexImage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations);
	}
#endif // glTexImage3DMultisample

#ifdef glTexImage3DMultisampleCoverageNV
#undef glTexImage3DMultisampleCoverageNV
	static inline void glTexImage3DMultisampleCoverageNV(GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations){
	    glad_debug_glTexImage3DMultisampleCoverageNV(target, coverageSamples, colorSamples, internalFormat, width, height, depth, fixedSampleLocations);
	}
#endif // glTexImage3DMultisampleCoverageNV

#ifdef glTexPageCommitmentARB
#undef glTexPageCommitmentARB
	static inline void glTexPageCommitmentARB(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit){
	    glad_debug_glTexPageCommitmentARB(target, level, xoffset, yoffset, zoffset, width, height, depth, commit);
	}
#endif // glTexPageCommitmentARB

#ifdef glTexPageCommitmentEXT
#undef glTexPageCommitmentEXT
	static inline void glTexPageCommitmentEXT(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit){
	    glad_debug_glTexPageCommitmentEXT(target, level, xoffset, yoffset, zoffset, width, height, depth, commit);
	}
#endif // glTexPageCommitmentEXT

#ifdef glTexPageCommitmentMemNV
#undef glTexPageCommitmentMemNV
	static inline void glTexPageCommitmentMemNV(GLenum target, GLint layer, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset, GLboolean commit){
	    glad_debug_glTexPageCommitmentMemNV(target, layer, level, xoffset, yoffset, zoffset, width, height, depth, memory, offset, commit);
	}
#endif // glTexPageCommitmentMemNV

#ifdef glTexParameterIiv
#undef glTexParameterIiv
	static inline void glTexParameterIiv(GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glTexParameterIiv(target, pname, params);
	}
#endif // glTexParameterIiv

#ifdef glTexParameterIivEXT
#undef glTexParameterIivEXT
	static inline void glTexParameterIivEXT(GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glTexParameterIivEXT(target, pname, params);
	}
#endif // glTexParameterIivEXT

#ifdef glTexParameterIivOES
#undef glTexParameterIivOES
	static inline void glTexParameterIivOES(GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glTexParameterIivOES(target, pname, params);
	}
#endif // glTexParameterIivOES

#ifdef glTexParameterIuiv
#undef glTexParameterIuiv
	static inline void glTexParameterIuiv(GLenum target, GLenum pname, const GLuint *params){
	    glad_debug_glTexParameterIuiv(target, pname, params);
	}
#endif // glTexParameterIuiv

#ifdef glTexParameterIuivEXT
#undef glTexParameterIuivEXT
	static inline void glTexParameterIuivEXT(GLenum target, GLenum pname, const GLuint *params){
	    glad_debug_glTexParameterIuivEXT(target, pname, params);
	}
#endif // glTexParameterIuivEXT

#ifdef glTexParameterIuivOES
#undef glTexParameterIuivOES
	static inline void glTexParameterIuivOES(GLenum target, GLenum pname, const GLuint *params){
	    glad_debug_glTexParameterIuivOES(target, pname, params);
	}
#endif // glTexParameterIuivOES

#ifdef glTexParameterf
#undef glTexParameterf
	static inline void glTexParameterf(GLenum target, GLenum pname, GLfloat param){
	    glad_debug_glTexParameterf(target, pname, param);
	}
#endif // glTexParameterf

#ifdef glTexParameterfv
#undef glTexParameterfv
	static inline void glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params){
	    glad_debug_glTexParameterfv(target, pname, params);
	}
#endif // glTexParameterfv

#ifdef glTexParameteri
#undef glTexParameteri
	static inline void glTexParameteri(GLenum target, GLenum pname, GLint param){
	    glad_debug_glTexParameteri(target, pname, param);
	}
#endif // glTexParameteri

#ifdef glTexParameteriv
#undef glTexParameteriv
	static inline void glTexParameteriv(GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glTexParameteriv(target, pname, params);
	}
#endif // glTexParameteriv

#ifdef glTexParameterx
#undef glTexParameterx
	static inline void glTexParameterx(GLenum target, GLenum pname, GLfixed param){
	    glad_debug_glTexParameterx(target, pname, param);
	}
#endif // glTexParameterx

#ifdef glTexParameterxOES
#undef glTexParameterxOES
	static inline void glTexParameterxOES(GLenum target, GLenum pname, GLfixed param){
	    glad_debug_glTexParameterxOES(target, pname, param);
	}
#endif // glTexParameterxOES

#ifdef glTexParameterxv
#undef glTexParameterxv
	static inline void glTexParameterxv(GLenum target, GLenum pname, const GLfixed *params){
	    glad_debug_glTexParameterxv(target, pname, params);
	}
#endif // glTexParameterxv

#ifdef glTexParameterxvOES
#undef glTexParameterxvOES
	static inline void glTexParameterxvOES(GLenum target, GLenum pname, const GLfixed *params){
	    glad_debug_glTexParameterxvOES(target, pname, params);
	}
#endif // glTexParameterxvOES

#ifdef glTexRenderbufferNV
#undef glTexRenderbufferNV
	static inline void glTexRenderbufferNV(GLenum target, GLuint renderbuffer){
	    glad_debug_glTexRenderbufferNV(target, renderbuffer);
	}
#endif // glTexRenderbufferNV

#ifdef glTexStorage1D
#undef glTexStorage1D
	static inline void glTexStorage1D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width){
	    glad_debug_glTexStorage1D(target, levels, internalformat, width);
	}
#endif // glTexStorage1D

#ifdef glTexStorage1DEXT
#undef glTexStorage1DEXT
	static inline void glTexStorage1DEXT(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width){
	    glad_debug_glTexStorage1DEXT(target, levels, internalformat, width);
	}
#endif // glTexStorage1DEXT

#ifdef glTexStorage2D
#undef glTexStorage2D
	static inline void glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glTexStorage2D(target, levels, internalformat, width, height);
	}
#endif // glTexStorage2D

#ifdef glTexStorage2DEXT
#undef glTexStorage2DEXT
	static inline void glTexStorage2DEXT(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glTexStorage2DEXT(target, levels, internalformat, width, height);
	}
#endif // glTexStorage2DEXT

#ifdef glTexStorage2DMultisample
#undef glTexStorage2DMultisample
	static inline void glTexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations){
	    glad_debug_glTexStorage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
	}
#endif // glTexStorage2DMultisample

#ifdef glTexStorage3D
#undef glTexStorage3D
	static inline void glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth){
	    glad_debug_glTexStorage3D(target, levels, internalformat, width, height, depth);
	}
#endif // glTexStorage3D

#ifdef glTexStorage3DEXT
#undef glTexStorage3DEXT
	static inline void glTexStorage3DEXT(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth){
	    glad_debug_glTexStorage3DEXT(target, levels, internalformat, width, height, depth);
	}
#endif // glTexStorage3DEXT

#ifdef glTexStorage3DMultisample
#undef glTexStorage3DMultisample
	static inline void glTexStorage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations){
	    glad_debug_glTexStorage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations);
	}
#endif // glTexStorage3DMultisample

#ifdef glTexStorage3DMultisampleOES
#undef glTexStorage3DMultisampleOES
	static inline void glTexStorage3DMultisampleOES(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations){
	    glad_debug_glTexStorage3DMultisampleOES(target, samples, internalformat, width, height, depth, fixedsamplelocations);
	}
#endif // glTexStorage3DMultisampleOES

#ifdef glTexStorageAttribs2DEXT
#undef glTexStorageAttribs2DEXT
	static inline void glTexStorageAttribs2DEXT(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, const GLint* attrib_list){
	    glad_debug_glTexStorageAttribs2DEXT(target, levels, internalformat, width, height, attrib_list);
	}
#endif // glTexStorageAttribs2DEXT

#ifdef glTexStorageAttribs3DEXT
#undef glTexStorageAttribs3DEXT
	static inline void glTexStorageAttribs3DEXT(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, const GLint* attrib_list){
	    glad_debug_glTexStorageAttribs3DEXT(target, levels, internalformat, width, height, depth, attrib_list);
	}
#endif // glTexStorageAttribs3DEXT

#ifdef glTexStorageMem1DEXT
#undef glTexStorageMem1DEXT
	static inline void glTexStorageMem1DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset){
	    glad_debug_glTexStorageMem1DEXT(target, levels, internalFormat, width, memory, offset);
	}
#endif // glTexStorageMem1DEXT

#ifdef glTexStorageMem2DEXT
#undef glTexStorageMem2DEXT
	static inline void glTexStorageMem2DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset){
	    glad_debug_glTexStorageMem2DEXT(target, levels, internalFormat, width, height, memory, offset);
	}
#endif // glTexStorageMem2DEXT

#ifdef glTexStorageMem2DMultisampleEXT
#undef glTexStorageMem2DMultisampleEXT
	static inline void glTexStorageMem2DMultisampleEXT(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset){
	    glad_debug_glTexStorageMem2DMultisampleEXT(target, samples, internalFormat, width, height, fixedSampleLocations, memory, offset);
	}
#endif // glTexStorageMem2DMultisampleEXT

#ifdef glTexStorageMem3DEXT
#undef glTexStorageMem3DEXT
	static inline void glTexStorageMem3DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset){
	    glad_debug_glTexStorageMem3DEXT(target, levels, internalFormat, width, height, depth, memory, offset);
	}
#endif // glTexStorageMem3DEXT

#ifdef glTexStorageMem3DMultisampleEXT
#undef glTexStorageMem3DMultisampleEXT
	static inline void glTexStorageMem3DMultisampleEXT(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset){
	    glad_debug_glTexStorageMem3DMultisampleEXT(target, samples, internalFormat, width, height, depth, fixedSampleLocations, memory, offset);
	}
#endif // glTexStorageMem3DMultisampleEXT

#ifdef glTexStorageSparseAMD
#undef glTexStorageSparseAMD
	static inline void glTexStorageSparseAMD(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags){
	    glad_debug_glTexStorageSparseAMD(target, internalFormat, width, height, depth, layers, flags);
	}
#endif // glTexStorageSparseAMD

#ifdef glTextureAttachMemoryNV
#undef glTextureAttachMemoryNV
	static inline void glTextureAttachMemoryNV(GLuint texture, GLuint memory, GLuint64 offset){
	    glad_debug_glTextureAttachMemoryNV(texture, memory, offset);
	}
#endif // glTextureAttachMemoryNV

#ifdef glTextureBarrier
#undef glTextureBarrier
	static inline void glTextureBarrier(){
	    glad_debug_glTextureBarrier();
	}
#endif // glTextureBarrier

#ifdef glTextureBarrierNV
#undef glTextureBarrierNV
	static inline void glTextureBarrierNV(){
	    glad_debug_glTextureBarrierNV();
	}
#endif // glTextureBarrierNV

#ifdef glTextureBuffer
#undef glTextureBuffer
	static inline void glTextureBuffer(GLuint texture, GLenum internalformat, GLuint buffer){
	    glad_debug_glTextureBuffer(texture, internalformat, buffer);
	}
#endif // glTextureBuffer

#ifdef glTextureBufferEXT
#undef glTextureBufferEXT
	static inline void glTextureBufferEXT(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer){
	    glad_debug_glTextureBufferEXT(texture, target, internalformat, buffer);
	}
#endif // glTextureBufferEXT

#ifdef glTextureBufferRange
#undef glTextureBufferRange
	static inline void glTextureBufferRange(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size){
	    glad_debug_glTextureBufferRange(texture, internalformat, buffer, offset, size);
	}
#endif // glTextureBufferRange

#ifdef glTextureBufferRangeEXT
#undef glTextureBufferRangeEXT
	static inline void glTextureBufferRangeEXT(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size){
	    glad_debug_glTextureBufferRangeEXT(texture, target, internalformat, buffer, offset, size);
	}
#endif // glTextureBufferRangeEXT

#ifdef glTextureColorMaskSGIS
#undef glTextureColorMaskSGIS
	static inline void glTextureColorMaskSGIS(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha){
	    glad_debug_glTextureColorMaskSGIS(red, green, blue, alpha);
	}
#endif // glTextureColorMaskSGIS

#ifdef glTextureFoveationParametersQCOM
#undef glTextureFoveationParametersQCOM
	static inline void glTextureFoveationParametersQCOM(GLuint texture, GLuint layer, GLuint focalPoint, GLfloat focalX, GLfloat focalY, GLfloat gainX, GLfloat gainY, GLfloat foveaArea){
	    glad_debug_glTextureFoveationParametersQCOM(texture, layer, focalPoint, focalX, focalY, gainX, gainY, foveaArea);
	}
#endif // glTextureFoveationParametersQCOM

#ifdef glTextureImage2DMultisampleCoverageNV
#undef glTextureImage2DMultisampleCoverageNV
	static inline void glTextureImage2DMultisampleCoverageNV(GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations){
	    glad_debug_glTextureImage2DMultisampleCoverageNV(texture, target, coverageSamples, colorSamples, internalFormat, width, height, fixedSampleLocations);
	}
#endif // glTextureImage2DMultisampleCoverageNV

#ifdef glTextureImage2DMultisampleNV
#undef glTextureImage2DMultisampleNV
	static inline void glTextureImage2DMultisampleNV(GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations){
	    glad_debug_glTextureImage2DMultisampleNV(texture, target, samples, internalFormat, width, height, fixedSampleLocations);
	}
#endif // glTextureImage2DMultisampleNV

#ifdef glTextureImage3DMultisampleCoverageNV
#undef glTextureImage3DMultisampleCoverageNV
	static inline void glTextureImage3DMultisampleCoverageNV(GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations){
	    glad_debug_glTextureImage3DMultisampleCoverageNV(texture, target, coverageSamples, colorSamples, internalFormat, width, height, depth, fixedSampleLocations);
	}
#endif // glTextureImage3DMultisampleCoverageNV

#ifdef glTextureImage3DMultisampleNV
#undef glTextureImage3DMultisampleNV
	static inline void glTextureImage3DMultisampleNV(GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations){
	    glad_debug_glTextureImage3DMultisampleNV(texture, target, samples, internalFormat, width, height, depth, fixedSampleLocations);
	}
#endif // glTextureImage3DMultisampleNV

#ifdef glTextureLightEXT
#undef glTextureLightEXT
	static inline void glTextureLightEXT(GLenum pname){
	    glad_debug_glTextureLightEXT(pname);
	}
#endif // glTextureLightEXT

#ifdef glTextureMaterialEXT
#undef glTextureMaterialEXT
	static inline void glTextureMaterialEXT(GLenum face, GLenum mode){
	    glad_debug_glTextureMaterialEXT(face, mode);
	}
#endif // glTextureMaterialEXT

#ifdef glTextureNormalEXT
#undef glTextureNormalEXT
	static inline void glTextureNormalEXT(GLenum mode){
	    glad_debug_glTextureNormalEXT(mode);
	}
#endif // glTextureNormalEXT

#ifdef glTexturePageCommitmentEXT
#undef glTexturePageCommitmentEXT
	static inline void glTexturePageCommitmentEXT(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit){
	    glad_debug_glTexturePageCommitmentEXT(texture, level, xoffset, yoffset, zoffset, width, height, depth, commit);
	}
#endif // glTexturePageCommitmentEXT

#ifdef glTexturePageCommitmentMemNV
#undef glTexturePageCommitmentMemNV
	static inline void glTexturePageCommitmentMemNV(GLuint texture, GLint layer, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset, GLboolean commit){
	    glad_debug_glTexturePageCommitmentMemNV(texture, layer, level, xoffset, yoffset, zoffset, width, height, depth, memory, offset, commit);
	}
#endif // glTexturePageCommitmentMemNV

#ifdef glTextureParameterIiv
#undef glTextureParameterIiv
	static inline void glTextureParameterIiv(GLuint texture, GLenum pname, const GLint *params){
	    glad_debug_glTextureParameterIiv(texture, pname, params);
	}
#endif // glTextureParameterIiv

#ifdef glTextureParameterIivEXT
#undef glTextureParameterIivEXT
	static inline void glTextureParameterIivEXT(GLuint texture, GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glTextureParameterIivEXT(texture, target, pname, params);
	}
#endif // glTextureParameterIivEXT

#ifdef glTextureParameterIuiv
#undef glTextureParameterIuiv
	static inline void glTextureParameterIuiv(GLuint texture, GLenum pname, const GLuint *params){
	    glad_debug_glTextureParameterIuiv(texture, pname, params);
	}
#endif // glTextureParameterIuiv

#ifdef glTextureParameterIuivEXT
#undef glTextureParameterIuivEXT
	static inline void glTextureParameterIuivEXT(GLuint texture, GLenum target, GLenum pname, const GLuint *params){
	    glad_debug_glTextureParameterIuivEXT(texture, target, pname, params);
	}
#endif // glTextureParameterIuivEXT

#ifdef glTextureParameterf
#undef glTextureParameterf
	static inline void glTextureParameterf(GLuint texture, GLenum pname, GLfloat param){
	    glad_debug_glTextureParameterf(texture, pname, param);
	}
#endif // glTextureParameterf

#ifdef glTextureParameterfEXT
#undef glTextureParameterfEXT
	static inline void glTextureParameterfEXT(GLuint texture, GLenum target, GLenum pname, GLfloat param){
	    glad_debug_glTextureParameterfEXT(texture, target, pname, param);
	}
#endif // glTextureParameterfEXT

#ifdef glTextureParameterfv
#undef glTextureParameterfv
	static inline void glTextureParameterfv(GLuint texture, GLenum pname, const GLfloat *param){
	    glad_debug_glTextureParameterfv(texture, pname, param);
	}
#endif // glTextureParameterfv

#ifdef glTextureParameterfvEXT
#undef glTextureParameterfvEXT
	static inline void glTextureParameterfvEXT(GLuint texture, GLenum target, GLenum pname, const GLfloat *params){
	    glad_debug_glTextureParameterfvEXT(texture, target, pname, params);
	}
#endif // glTextureParameterfvEXT

#ifdef glTextureParameteri
#undef glTextureParameteri
	static inline void glTextureParameteri(GLuint texture, GLenum pname, GLint param){
	    glad_debug_glTextureParameteri(texture, pname, param);
	}
#endif // glTextureParameteri

#ifdef glTextureParameteriEXT
#undef glTextureParameteriEXT
	static inline void glTextureParameteriEXT(GLuint texture, GLenum target, GLenum pname, GLint param){
	    glad_debug_glTextureParameteriEXT(texture, target, pname, param);
	}
#endif // glTextureParameteriEXT

#ifdef glTextureParameteriv
#undef glTextureParameteriv
	static inline void glTextureParameteriv(GLuint texture, GLenum pname, const GLint *param){
	    glad_debug_glTextureParameteriv(texture, pname, param);
	}
#endif // glTextureParameteriv

#ifdef glTextureParameterivEXT
#undef glTextureParameterivEXT
	static inline void glTextureParameterivEXT(GLuint texture, GLenum target, GLenum pname, const GLint *params){
	    glad_debug_glTextureParameterivEXT(texture, target, pname, params);
	}
#endif // glTextureParameterivEXT

#ifdef glTextureRenderbufferEXT
#undef glTextureRenderbufferEXT
	static inline void glTextureRenderbufferEXT(GLuint texture, GLenum target, GLuint renderbuffer){
	    glad_debug_glTextureRenderbufferEXT(texture, target, renderbuffer);
	}
#endif // glTextureRenderbufferEXT

#ifdef glTextureStorage1D
#undef glTextureStorage1D
	static inline void glTextureStorage1D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width){
	    glad_debug_glTextureStorage1D(texture, levels, internalformat, width);
	}
#endif // glTextureStorage1D

#ifdef glTextureStorage1DEXT
#undef glTextureStorage1DEXT
	static inline void glTextureStorage1DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width){
	    glad_debug_glTextureStorage1DEXT(texture, target, levels, internalformat, width);
	}
#endif // glTextureStorage1DEXT

#ifdef glTextureStorage2D
#undef glTextureStorage2D
	static inline void glTextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glTextureStorage2D(texture, levels, internalformat, width, height);
	}
#endif // glTextureStorage2D

#ifdef glTextureStorage2DEXT
#undef glTextureStorage2DEXT
	static inline void glTextureStorage2DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height){
	    glad_debug_glTextureStorage2DEXT(texture, target, levels, internalformat, width, height);
	}
#endif // glTextureStorage2DEXT

#ifdef glTextureStorage2DMultisample
#undef glTextureStorage2DMultisample
	static inline void glTextureStorage2DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations){
	    glad_debug_glTextureStorage2DMultisample(texture, samples, internalformat, width, height, fixedsamplelocations);
	}
#endif // glTextureStorage2DMultisample

#ifdef glTextureStorage2DMultisampleEXT
#undef glTextureStorage2DMultisampleEXT
	static inline void glTextureStorage2DMultisampleEXT(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations){
	    glad_debug_glTextureStorage2DMultisampleEXT(texture, target, samples, internalformat, width, height, fixedsamplelocations);
	}
#endif // glTextureStorage2DMultisampleEXT

#ifdef glTextureStorage3D
#undef glTextureStorage3D
	static inline void glTextureStorage3D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth){
	    glad_debug_glTextureStorage3D(texture, levels, internalformat, width, height, depth);
	}
#endif // glTextureStorage3D

#ifdef glTextureStorage3DEXT
#undef glTextureStorage3DEXT
	static inline void glTextureStorage3DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth){
	    glad_debug_glTextureStorage3DEXT(texture, target, levels, internalformat, width, height, depth);
	}
#endif // glTextureStorage3DEXT

#ifdef glTextureStorage3DMultisample
#undef glTextureStorage3DMultisample
	static inline void glTextureStorage3DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations){
	    glad_debug_glTextureStorage3DMultisample(texture, samples, internalformat, width, height, depth, fixedsamplelocations);
	}
#endif // glTextureStorage3DMultisample

#ifdef glTextureStorage3DMultisampleEXT
#undef glTextureStorage3DMultisampleEXT
	static inline void glTextureStorage3DMultisampleEXT(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations){
	    glad_debug_glTextureStorage3DMultisampleEXT(texture, target, samples, internalformat, width, height, depth, fixedsamplelocations);
	}
#endif // glTextureStorage3DMultisampleEXT

#ifdef glTextureStorageMem1DEXT
#undef glTextureStorageMem1DEXT
	static inline void glTextureStorageMem1DEXT(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset){
	    glad_debug_glTextureStorageMem1DEXT(texture, levels, internalFormat, width, memory, offset);
	}
#endif // glTextureStorageMem1DEXT

#ifdef glTextureStorageMem2DEXT
#undef glTextureStorageMem2DEXT
	static inline void glTextureStorageMem2DEXT(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset){
	    glad_debug_glTextureStorageMem2DEXT(texture, levels, internalFormat, width, height, memory, offset);
	}
#endif // glTextureStorageMem2DEXT

#ifdef glTextureStorageMem2DMultisampleEXT
#undef glTextureStorageMem2DMultisampleEXT
	static inline void glTextureStorageMem2DMultisampleEXT(GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset){
	    glad_debug_glTextureStorageMem2DMultisampleEXT(texture, samples, internalFormat, width, height, fixedSampleLocations, memory, offset);
	}
#endif // glTextureStorageMem2DMultisampleEXT

#ifdef glTextureStorageMem3DEXT
#undef glTextureStorageMem3DEXT
	static inline void glTextureStorageMem3DEXT(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset){
	    glad_debug_glTextureStorageMem3DEXT(texture, levels, internalFormat, width, height, depth, memory, offset);
	}
#endif // glTextureStorageMem3DEXT

#ifdef glTextureStorageMem3DMultisampleEXT
#undef glTextureStorageMem3DMultisampleEXT
	static inline void glTextureStorageMem3DMultisampleEXT(GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset){
	    glad_debug_glTextureStorageMem3DMultisampleEXT(texture, samples, internalFormat, width, height, depth, fixedSampleLocations, memory, offset);
	}
#endif // glTextureStorageMem3DMultisampleEXT

#ifdef glTextureStorageSparseAMD
#undef glTextureStorageSparseAMD
	static inline void glTextureStorageSparseAMD(GLuint texture, GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags){
	    glad_debug_glTextureStorageSparseAMD(texture, target, internalFormat, width, height, depth, layers, flags);
	}
#endif // glTextureStorageSparseAMD

#ifdef glTextureView
#undef glTextureView
	static inline void glTextureView(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers){
	    glad_debug_glTextureView(texture, target, origtexture, internalformat, minlevel, numlevels, minlayer, numlayers);
	}
#endif // glTextureView

#ifdef glTextureViewEXT
#undef glTextureViewEXT
	static inline void glTextureViewEXT(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers){
	    glad_debug_glTextureViewEXT(texture, target, origtexture, internalformat, minlevel, numlevels, minlayer, numlayers);
	}
#endif // glTextureViewEXT

#ifdef glTextureViewOES
#undef glTextureViewOES
	static inline void glTextureViewOES(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers){
	    glad_debug_glTextureViewOES(texture, target, origtexture, internalformat, minlevel, numlevels, minlayer, numlayers);
	}
#endif // glTextureViewOES

#ifdef glTrackMatrixNV
#undef glTrackMatrixNV
	static inline void glTrackMatrixNV(GLenum target, GLuint address, GLenum matrix, GLenum transform){
	    glad_debug_glTrackMatrixNV(target, address, matrix, transform);
	}
#endif // glTrackMatrixNV

#ifdef glTransformFeedbackAttribsNV
#undef glTransformFeedbackAttribsNV
	static inline void glTransformFeedbackAttribsNV(GLsizei count, const GLint *attribs, GLenum bufferMode){
	    glad_debug_glTransformFeedbackAttribsNV(count, attribs, bufferMode);
	}
#endif // glTransformFeedbackAttribsNV

#ifdef glTransformFeedbackBufferBase
#undef glTransformFeedbackBufferBase
	static inline void glTransformFeedbackBufferBase(GLuint xfb, GLuint index, GLuint buffer){
	    glad_debug_glTransformFeedbackBufferBase(xfb, index, buffer);
	}
#endif // glTransformFeedbackBufferBase

#ifdef glTransformFeedbackBufferRange
#undef glTransformFeedbackBufferRange
	static inline void glTransformFeedbackBufferRange(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size){
	    glad_debug_glTransformFeedbackBufferRange(xfb, index, buffer, offset, size);
	}
#endif // glTransformFeedbackBufferRange

#ifdef glTransformFeedbackStreamAttribsNV
#undef glTransformFeedbackStreamAttribsNV
	static inline void glTransformFeedbackStreamAttribsNV(GLsizei count, const GLint *attribs, GLsizei nbuffers, const GLint *bufstreams, GLenum bufferMode){
	    glad_debug_glTransformFeedbackStreamAttribsNV(count, attribs, nbuffers, bufstreams, bufferMode);
	}
#endif // glTransformFeedbackStreamAttribsNV

#ifdef glTransformFeedbackVaryings
#undef glTransformFeedbackVaryings
	static inline void glTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode){
	    glad_debug_glTransformFeedbackVaryings(program, count, varyings, bufferMode);
	}
#endif // glTransformFeedbackVaryings

#ifdef glTransformFeedbackVaryingsEXT
#undef glTransformFeedbackVaryingsEXT
	static inline void glTransformFeedbackVaryingsEXT(GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode){
	    glad_debug_glTransformFeedbackVaryingsEXT(program, count, varyings, bufferMode);
	}
#endif // glTransformFeedbackVaryingsEXT

#ifdef glTransformFeedbackVaryingsNV
#undef glTransformFeedbackVaryingsNV
	static inline void glTransformFeedbackVaryingsNV(GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode){
	    glad_debug_glTransformFeedbackVaryingsNV(program, count, locations, bufferMode);
	}
#endif // glTransformFeedbackVaryingsNV

#ifdef glTransformPathNV
#undef glTransformPathNV
	static inline void glTransformPathNV(GLuint resultPath, GLuint srcPath, GLenum transformType, const GLfloat *transformValues){
	    glad_debug_glTransformPathNV(resultPath, srcPath, transformType, transformValues);
	}
#endif // glTransformPathNV

#ifdef glTranslated
#undef glTranslated
	static inline void glTranslated(GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glTranslated(x, y, z);
	}
#endif // glTranslated

#ifdef glTranslatef
#undef glTranslatef
	static inline void glTranslatef(GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glTranslatef(x, y, z);
	}
#endif // glTranslatef

#ifdef glTranslatex
#undef glTranslatex
	static inline void glTranslatex(GLfixed x, GLfixed y, GLfixed z){
	    glad_debug_glTranslatex(x, y, z);
	}
#endif // glTranslatex

#ifdef glTranslatexOES
#undef glTranslatexOES
	static inline void glTranslatexOES(GLfixed x, GLfixed y, GLfixed z){
	    glad_debug_glTranslatexOES(x, y, z);
	}
#endif // glTranslatexOES

#ifdef glUniform1d
#undef glUniform1d
	static inline void glUniform1d(GLint location, GLdouble x){
	    glad_debug_glUniform1d(location, x);
	}
#endif // glUniform1d

#ifdef glUniform1dv
#undef glUniform1dv
	static inline void glUniform1dv(GLint location, GLsizei count, const GLdouble *value){
	    glad_debug_glUniform1dv(location, count, value);
	}
#endif // glUniform1dv

#ifdef glUniform1f
#undef glUniform1f
	static inline void glUniform1f(GLint location, GLfloat v0){
	    glad_debug_glUniform1f(location, v0);
	}
#endif // glUniform1f

#ifdef glUniform1fARB
#undef glUniform1fARB
	static inline void glUniform1fARB(GLint location, GLfloat v0){
	    glad_debug_glUniform1fARB(location, v0);
	}
#endif // glUniform1fARB

#ifdef glUniform1fv
#undef glUniform1fv
	static inline void glUniform1fv(GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glUniform1fv(location, count, value);
	}
#endif // glUniform1fv

#ifdef glUniform1fvARB
#undef glUniform1fvARB
	static inline void glUniform1fvARB(GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glUniform1fvARB(location, count, value);
	}
#endif // glUniform1fvARB

#ifdef glUniform1i
#undef glUniform1i
	static inline void glUniform1i(GLint location, GLint v0){
	    glad_debug_glUniform1i(location, v0);
	}
#endif // glUniform1i

#ifdef glUniform1i64ARB
#undef glUniform1i64ARB
	static inline void glUniform1i64ARB(GLint location, GLint64 x){
	    glad_debug_glUniform1i64ARB(location, x);
	}
#endif // glUniform1i64ARB

#ifdef glUniform1i64NV
#undef glUniform1i64NV
	static inline void glUniform1i64NV(GLint location, GLint64EXT x){
	    glad_debug_glUniform1i64NV(location, x);
	}
#endif // glUniform1i64NV

#ifdef glUniform1i64vARB
#undef glUniform1i64vARB
	static inline void glUniform1i64vARB(GLint location, GLsizei count, const GLint64 *value){
	    glad_debug_glUniform1i64vARB(location, count, value);
	}
#endif // glUniform1i64vARB

#ifdef glUniform1i64vNV
#undef glUniform1i64vNV
	static inline void glUniform1i64vNV(GLint location, GLsizei count, const GLint64EXT *value){
	    glad_debug_glUniform1i64vNV(location, count, value);
	}
#endif // glUniform1i64vNV

#ifdef glUniform1iARB
#undef glUniform1iARB
	static inline void glUniform1iARB(GLint location, GLint v0){
	    glad_debug_glUniform1iARB(location, v0);
	}
#endif // glUniform1iARB

#ifdef glUniform1iv
#undef glUniform1iv
	static inline void glUniform1iv(GLint location, GLsizei count, const GLint *value){
	    glad_debug_glUniform1iv(location, count, value);
	}
#endif // glUniform1iv

#ifdef glUniform1ivARB
#undef glUniform1ivARB
	static inline void glUniform1ivARB(GLint location, GLsizei count, const GLint *value){
	    glad_debug_glUniform1ivARB(location, count, value);
	}
#endif // glUniform1ivARB

#ifdef glUniform1ui
#undef glUniform1ui
	static inline void glUniform1ui(GLint location, GLuint v0){
	    glad_debug_glUniform1ui(location, v0);
	}
#endif // glUniform1ui

#ifdef glUniform1ui64ARB
#undef glUniform1ui64ARB
	static inline void glUniform1ui64ARB(GLint location, GLuint64 x){
	    glad_debug_glUniform1ui64ARB(location, x);
	}
#endif // glUniform1ui64ARB

#ifdef glUniform1ui64NV
#undef glUniform1ui64NV
	static inline void glUniform1ui64NV(GLint location, GLuint64EXT x){
	    glad_debug_glUniform1ui64NV(location, x);
	}
#endif // glUniform1ui64NV

#ifdef glUniform1ui64vARB
#undef glUniform1ui64vARB
	static inline void glUniform1ui64vARB(GLint location, GLsizei count, const GLuint64 *value){
	    glad_debug_glUniform1ui64vARB(location, count, value);
	}
#endif // glUniform1ui64vARB

#ifdef glUniform1ui64vNV
#undef glUniform1ui64vNV
	static inline void glUniform1ui64vNV(GLint location, GLsizei count, const GLuint64EXT *value){
	    glad_debug_glUniform1ui64vNV(location, count, value);
	}
#endif // glUniform1ui64vNV

#ifdef glUniform1uiEXT
#undef glUniform1uiEXT
	static inline void glUniform1uiEXT(GLint location, GLuint v0){
	    glad_debug_glUniform1uiEXT(location, v0);
	}
#endif // glUniform1uiEXT

#ifdef glUniform1uiv
#undef glUniform1uiv
	static inline void glUniform1uiv(GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glUniform1uiv(location, count, value);
	}
#endif // glUniform1uiv

#ifdef glUniform1uivEXT
#undef glUniform1uivEXT
	static inline void glUniform1uivEXT(GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glUniform1uivEXT(location, count, value);
	}
#endif // glUniform1uivEXT

#ifdef glUniform2d
#undef glUniform2d
	static inline void glUniform2d(GLint location, GLdouble x, GLdouble y){
	    glad_debug_glUniform2d(location, x, y);
	}
#endif // glUniform2d

#ifdef glUniform2dv
#undef glUniform2dv
	static inline void glUniform2dv(GLint location, GLsizei count, const GLdouble *value){
	    glad_debug_glUniform2dv(location, count, value);
	}
#endif // glUniform2dv

#ifdef glUniform2f
#undef glUniform2f
	static inline void glUniform2f(GLint location, GLfloat v0, GLfloat v1){
	    glad_debug_glUniform2f(location, v0, v1);
	}
#endif // glUniform2f

#ifdef glUniform2fARB
#undef glUniform2fARB
	static inline void glUniform2fARB(GLint location, GLfloat v0, GLfloat v1){
	    glad_debug_glUniform2fARB(location, v0, v1);
	}
#endif // glUniform2fARB

#ifdef glUniform2fv
#undef glUniform2fv
	static inline void glUniform2fv(GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glUniform2fv(location, count, value);
	}
#endif // glUniform2fv

#ifdef glUniform2fvARB
#undef glUniform2fvARB
	static inline void glUniform2fvARB(GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glUniform2fvARB(location, count, value);
	}
#endif // glUniform2fvARB

#ifdef glUniform2i
#undef glUniform2i
	static inline void glUniform2i(GLint location, GLint v0, GLint v1){
	    glad_debug_glUniform2i(location, v0, v1);
	}
#endif // glUniform2i

#ifdef glUniform2i64ARB
#undef glUniform2i64ARB
	static inline void glUniform2i64ARB(GLint location, GLint64 x, GLint64 y){
	    glad_debug_glUniform2i64ARB(location, x, y);
	}
#endif // glUniform2i64ARB

#ifdef glUniform2i64NV
#undef glUniform2i64NV
	static inline void glUniform2i64NV(GLint location, GLint64EXT x, GLint64EXT y){
	    glad_debug_glUniform2i64NV(location, x, y);
	}
#endif // glUniform2i64NV

#ifdef glUniform2i64vARB
#undef glUniform2i64vARB
	static inline void glUniform2i64vARB(GLint location, GLsizei count, const GLint64 *value){
	    glad_debug_glUniform2i64vARB(location, count, value);
	}
#endif // glUniform2i64vARB

#ifdef glUniform2i64vNV
#undef glUniform2i64vNV
	static inline void glUniform2i64vNV(GLint location, GLsizei count, const GLint64EXT *value){
	    glad_debug_glUniform2i64vNV(location, count, value);
	}
#endif // glUniform2i64vNV

#ifdef glUniform2iARB
#undef glUniform2iARB
	static inline void glUniform2iARB(GLint location, GLint v0, GLint v1){
	    glad_debug_glUniform2iARB(location, v0, v1);
	}
#endif // glUniform2iARB

#ifdef glUniform2iv
#undef glUniform2iv
	static inline void glUniform2iv(GLint location, GLsizei count, const GLint *value){
	    glad_debug_glUniform2iv(location, count, value);
	}
#endif // glUniform2iv

#ifdef glUniform2ivARB
#undef glUniform2ivARB
	static inline void glUniform2ivARB(GLint location, GLsizei count, const GLint *value){
	    glad_debug_glUniform2ivARB(location, count, value);
	}
#endif // glUniform2ivARB

#ifdef glUniform2ui
#undef glUniform2ui
	static inline void glUniform2ui(GLint location, GLuint v0, GLuint v1){
	    glad_debug_glUniform2ui(location, v0, v1);
	}
#endif // glUniform2ui

#ifdef glUniform2ui64ARB
#undef glUniform2ui64ARB
	static inline void glUniform2ui64ARB(GLint location, GLuint64 x, GLuint64 y){
	    glad_debug_glUniform2ui64ARB(location, x, y);
	}
#endif // glUniform2ui64ARB

#ifdef glUniform2ui64NV
#undef glUniform2ui64NV
	static inline void glUniform2ui64NV(GLint location, GLuint64EXT x, GLuint64EXT y){
	    glad_debug_glUniform2ui64NV(location, x, y);
	}
#endif // glUniform2ui64NV

#ifdef glUniform2ui64vARB
#undef glUniform2ui64vARB
	static inline void glUniform2ui64vARB(GLint location, GLsizei count, const GLuint64 *value){
	    glad_debug_glUniform2ui64vARB(location, count, value);
	}
#endif // glUniform2ui64vARB

#ifdef glUniform2ui64vNV
#undef glUniform2ui64vNV
	static inline void glUniform2ui64vNV(GLint location, GLsizei count, const GLuint64EXT *value){
	    glad_debug_glUniform2ui64vNV(location, count, value);
	}
#endif // glUniform2ui64vNV

#ifdef glUniform2uiEXT
#undef glUniform2uiEXT
	static inline void glUniform2uiEXT(GLint location, GLuint v0, GLuint v1){
	    glad_debug_glUniform2uiEXT(location, v0, v1);
	}
#endif // glUniform2uiEXT

#ifdef glUniform2uiv
#undef glUniform2uiv
	static inline void glUniform2uiv(GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glUniform2uiv(location, count, value);
	}
#endif // glUniform2uiv

#ifdef glUniform2uivEXT
#undef glUniform2uivEXT
	static inline void glUniform2uivEXT(GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glUniform2uivEXT(location, count, value);
	}
#endif // glUniform2uivEXT

#ifdef glUniform3d
#undef glUniform3d
	static inline void glUniform3d(GLint location, GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glUniform3d(location, x, y, z);
	}
#endif // glUniform3d

#ifdef glUniform3dv
#undef glUniform3dv
	static inline void glUniform3dv(GLint location, GLsizei count, const GLdouble *value){
	    glad_debug_glUniform3dv(location, count, value);
	}
#endif // glUniform3dv

#ifdef glUniform3f
#undef glUniform3f
	static inline void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2){
	    glad_debug_glUniform3f(location, v0, v1, v2);
	}
#endif // glUniform3f

#ifdef glUniform3fARB
#undef glUniform3fARB
	static inline void glUniform3fARB(GLint location, GLfloat v0, GLfloat v1, GLfloat v2){
	    glad_debug_glUniform3fARB(location, v0, v1, v2);
	}
#endif // glUniform3fARB

#ifdef glUniform3fv
#undef glUniform3fv
	static inline void glUniform3fv(GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glUniform3fv(location, count, value);
	}
#endif // glUniform3fv

#ifdef glUniform3fvARB
#undef glUniform3fvARB
	static inline void glUniform3fvARB(GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glUniform3fvARB(location, count, value);
	}
#endif // glUniform3fvARB

#ifdef glUniform3i
#undef glUniform3i
	static inline void glUniform3i(GLint location, GLint v0, GLint v1, GLint v2){
	    glad_debug_glUniform3i(location, v0, v1, v2);
	}
#endif // glUniform3i

#ifdef glUniform3i64ARB
#undef glUniform3i64ARB
	static inline void glUniform3i64ARB(GLint location, GLint64 x, GLint64 y, GLint64 z){
	    glad_debug_glUniform3i64ARB(location, x, y, z);
	}
#endif // glUniform3i64ARB

#ifdef glUniform3i64NV
#undef glUniform3i64NV
	static inline void glUniform3i64NV(GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z){
	    glad_debug_glUniform3i64NV(location, x, y, z);
	}
#endif // glUniform3i64NV

#ifdef glUniform3i64vARB
#undef glUniform3i64vARB
	static inline void glUniform3i64vARB(GLint location, GLsizei count, const GLint64 *value){
	    glad_debug_glUniform3i64vARB(location, count, value);
	}
#endif // glUniform3i64vARB

#ifdef glUniform3i64vNV
#undef glUniform3i64vNV
	static inline void glUniform3i64vNV(GLint location, GLsizei count, const GLint64EXT *value){
	    glad_debug_glUniform3i64vNV(location, count, value);
	}
#endif // glUniform3i64vNV

#ifdef glUniform3iARB
#undef glUniform3iARB
	static inline void glUniform3iARB(GLint location, GLint v0, GLint v1, GLint v2){
	    glad_debug_glUniform3iARB(location, v0, v1, v2);
	}
#endif // glUniform3iARB

#ifdef glUniform3iv
#undef glUniform3iv
	static inline void glUniform3iv(GLint location, GLsizei count, const GLint *value){
	    glad_debug_glUniform3iv(location, count, value);
	}
#endif // glUniform3iv

#ifdef glUniform3ivARB
#undef glUniform3ivARB
	static inline void glUniform3ivARB(GLint location, GLsizei count, const GLint *value){
	    glad_debug_glUniform3ivARB(location, count, value);
	}
#endif // glUniform3ivARB

#ifdef glUniform3ui
#undef glUniform3ui
	static inline void glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2){
	    glad_debug_glUniform3ui(location, v0, v1, v2);
	}
#endif // glUniform3ui

#ifdef glUniform3ui64ARB
#undef glUniform3ui64ARB
	static inline void glUniform3ui64ARB(GLint location, GLuint64 x, GLuint64 y, GLuint64 z){
	    glad_debug_glUniform3ui64ARB(location, x, y, z);
	}
#endif // glUniform3ui64ARB

#ifdef glUniform3ui64NV
#undef glUniform3ui64NV
	static inline void glUniform3ui64NV(GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z){
	    glad_debug_glUniform3ui64NV(location, x, y, z);
	}
#endif // glUniform3ui64NV

#ifdef glUniform3ui64vARB
#undef glUniform3ui64vARB
	static inline void glUniform3ui64vARB(GLint location, GLsizei count, const GLuint64 *value){
	    glad_debug_glUniform3ui64vARB(location, count, value);
	}
#endif // glUniform3ui64vARB

#ifdef glUniform3ui64vNV
#undef glUniform3ui64vNV
	static inline void glUniform3ui64vNV(GLint location, GLsizei count, const GLuint64EXT *value){
	    glad_debug_glUniform3ui64vNV(location, count, value);
	}
#endif // glUniform3ui64vNV

#ifdef glUniform3uiEXT
#undef glUniform3uiEXT
	static inline void glUniform3uiEXT(GLint location, GLuint v0, GLuint v1, GLuint v2){
	    glad_debug_glUniform3uiEXT(location, v0, v1, v2);
	}
#endif // glUniform3uiEXT

#ifdef glUniform3uiv
#undef glUniform3uiv
	static inline void glUniform3uiv(GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glUniform3uiv(location, count, value);
	}
#endif // glUniform3uiv

#ifdef glUniform3uivEXT
#undef glUniform3uivEXT
	static inline void glUniform3uivEXT(GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glUniform3uivEXT(location, count, value);
	}
#endif // glUniform3uivEXT

#ifdef glUniform4d
#undef glUniform4d
	static inline void glUniform4d(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glUniform4d(location, x, y, z, w);
	}
#endif // glUniform4d

#ifdef glUniform4dv
#undef glUniform4dv
	static inline void glUniform4dv(GLint location, GLsizei count, const GLdouble *value){
	    glad_debug_glUniform4dv(location, count, value);
	}
#endif // glUniform4dv

#ifdef glUniform4f
#undef glUniform4f
	static inline void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3){
	    glad_debug_glUniform4f(location, v0, v1, v2, v3);
	}
#endif // glUniform4f

#ifdef glUniform4fARB
#undef glUniform4fARB
	static inline void glUniform4fARB(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3){
	    glad_debug_glUniform4fARB(location, v0, v1, v2, v3);
	}
#endif // glUniform4fARB

#ifdef glUniform4fv
#undef glUniform4fv
	static inline void glUniform4fv(GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glUniform4fv(location, count, value);
	}
#endif // glUniform4fv

#ifdef glUniform4fvARB
#undef glUniform4fvARB
	static inline void glUniform4fvARB(GLint location, GLsizei count, const GLfloat *value){
	    glad_debug_glUniform4fvARB(location, count, value);
	}
#endif // glUniform4fvARB

#ifdef glUniform4i
#undef glUniform4i
	static inline void glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3){
	    glad_debug_glUniform4i(location, v0, v1, v2, v3);
	}
#endif // glUniform4i

#ifdef glUniform4i64ARB
#undef glUniform4i64ARB
	static inline void glUniform4i64ARB(GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w){
	    glad_debug_glUniform4i64ARB(location, x, y, z, w);
	}
#endif // glUniform4i64ARB

#ifdef glUniform4i64NV
#undef glUniform4i64NV
	static inline void glUniform4i64NV(GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w){
	    glad_debug_glUniform4i64NV(location, x, y, z, w);
	}
#endif // glUniform4i64NV

#ifdef glUniform4i64vARB
#undef glUniform4i64vARB
	static inline void glUniform4i64vARB(GLint location, GLsizei count, const GLint64 *value){
	    glad_debug_glUniform4i64vARB(location, count, value);
	}
#endif // glUniform4i64vARB

#ifdef glUniform4i64vNV
#undef glUniform4i64vNV
	static inline void glUniform4i64vNV(GLint location, GLsizei count, const GLint64EXT *value){
	    glad_debug_glUniform4i64vNV(location, count, value);
	}
#endif // glUniform4i64vNV

#ifdef glUniform4iARB
#undef glUniform4iARB
	static inline void glUniform4iARB(GLint location, GLint v0, GLint v1, GLint v2, GLint v3){
	    glad_debug_glUniform4iARB(location, v0, v1, v2, v3);
	}
#endif // glUniform4iARB

#ifdef glUniform4iv
#undef glUniform4iv
	static inline void glUniform4iv(GLint location, GLsizei count, const GLint *value){
	    glad_debug_glUniform4iv(location, count, value);
	}
#endif // glUniform4iv

#ifdef glUniform4ivARB
#undef glUniform4ivARB
	static inline void glUniform4ivARB(GLint location, GLsizei count, const GLint *value){
	    glad_debug_glUniform4ivARB(location, count, value);
	}
#endif // glUniform4ivARB

#ifdef glUniform4ui
#undef glUniform4ui
	static inline void glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3){
	    glad_debug_glUniform4ui(location, v0, v1, v2, v3);
	}
#endif // glUniform4ui

#ifdef glUniform4ui64ARB
#undef glUniform4ui64ARB
	static inline void glUniform4ui64ARB(GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w){
	    glad_debug_glUniform4ui64ARB(location, x, y, z, w);
	}
#endif // glUniform4ui64ARB

#ifdef glUniform4ui64NV
#undef glUniform4ui64NV
	static inline void glUniform4ui64NV(GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w){
	    glad_debug_glUniform4ui64NV(location, x, y, z, w);
	}
#endif // glUniform4ui64NV

#ifdef glUniform4ui64vARB
#undef glUniform4ui64vARB
	static inline void glUniform4ui64vARB(GLint location, GLsizei count, const GLuint64 *value){
	    glad_debug_glUniform4ui64vARB(location, count, value);
	}
#endif // glUniform4ui64vARB

#ifdef glUniform4ui64vNV
#undef glUniform4ui64vNV
	static inline void glUniform4ui64vNV(GLint location, GLsizei count, const GLuint64EXT *value){
	    glad_debug_glUniform4ui64vNV(location, count, value);
	}
#endif // glUniform4ui64vNV

#ifdef glUniform4uiEXT
#undef glUniform4uiEXT
	static inline void glUniform4uiEXT(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3){
	    glad_debug_glUniform4uiEXT(location, v0, v1, v2, v3);
	}
#endif // glUniform4uiEXT

#ifdef glUniform4uiv
#undef glUniform4uiv
	static inline void glUniform4uiv(GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glUniform4uiv(location, count, value);
	}
#endif // glUniform4uiv

#ifdef glUniform4uivEXT
#undef glUniform4uivEXT
	static inline void glUniform4uivEXT(GLint location, GLsizei count, const GLuint *value){
	    glad_debug_glUniform4uivEXT(location, count, value);
	}
#endif // glUniform4uivEXT

#ifdef glUniformBlockBinding
#undef glUniformBlockBinding
	static inline void glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding){
	    glad_debug_glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
	}
#endif // glUniformBlockBinding

#ifdef glUniformBufferEXT
#undef glUniformBufferEXT
	static inline void glUniformBufferEXT(GLuint program, GLint location, GLuint buffer){
	    glad_debug_glUniformBufferEXT(program, location, buffer);
	}
#endif // glUniformBufferEXT

#ifdef glUniformHandleui64ARB
#undef glUniformHandleui64ARB
	static inline void glUniformHandleui64ARB(GLint location, GLuint64 value){
	    glad_debug_glUniformHandleui64ARB(location, value);
	}
#endif // glUniformHandleui64ARB

#ifdef glUniformHandleui64IMG
#undef glUniformHandleui64IMG
	static inline void glUniformHandleui64IMG(GLint location, GLuint64 value){
	    glad_debug_glUniformHandleui64IMG(location, value);
	}
#endif // glUniformHandleui64IMG

#ifdef glUniformHandleui64NV
#undef glUniformHandleui64NV
	static inline void glUniformHandleui64NV(GLint location, GLuint64 value){
	    glad_debug_glUniformHandleui64NV(location, value);
	}
#endif // glUniformHandleui64NV

#ifdef glUniformHandleui64vARB
#undef glUniformHandleui64vARB
	static inline void glUniformHandleui64vARB(GLint location, GLsizei count, const GLuint64 *value){
	    glad_debug_glUniformHandleui64vARB(location, count, value);
	}
#endif // glUniformHandleui64vARB

#ifdef glUniformHandleui64vIMG
#undef glUniformHandleui64vIMG
	static inline void glUniformHandleui64vIMG(GLint location, GLsizei count, const GLuint64 *value){
	    glad_debug_glUniformHandleui64vIMG(location, count, value);
	}
#endif // glUniformHandleui64vIMG

#ifdef glUniformHandleui64vNV
#undef glUniformHandleui64vNV
	static inline void glUniformHandleui64vNV(GLint location, GLsizei count, const GLuint64 *value){
	    glad_debug_glUniformHandleui64vNV(location, count, value);
	}
#endif // glUniformHandleui64vNV

#ifdef glUniformMatrix2dv
#undef glUniformMatrix2dv
	static inline void glUniformMatrix2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glUniformMatrix2dv(location, count, transpose, value);
	}
#endif // glUniformMatrix2dv

#ifdef glUniformMatrix2fv
#undef glUniformMatrix2fv
	static inline void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix2fv(location, count, transpose, value);
	}
#endif // glUniformMatrix2fv

#ifdef glUniformMatrix2fvARB
#undef glUniformMatrix2fvARB
	static inline void glUniformMatrix2fvARB(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix2fvARB(location, count, transpose, value);
	}
#endif // glUniformMatrix2fvARB

#ifdef glUniformMatrix2x3dv
#undef glUniformMatrix2x3dv
	static inline void glUniformMatrix2x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glUniformMatrix2x3dv(location, count, transpose, value);
	}
#endif // glUniformMatrix2x3dv

#ifdef glUniformMatrix2x3fv
#undef glUniformMatrix2x3fv
	static inline void glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix2x3fv(location, count, transpose, value);
	}
#endif // glUniformMatrix2x3fv

#ifdef glUniformMatrix2x3fvNV
#undef glUniformMatrix2x3fvNV
	static inline void glUniformMatrix2x3fvNV(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix2x3fvNV(location, count, transpose, value);
	}
#endif // glUniformMatrix2x3fvNV

#ifdef glUniformMatrix2x4dv
#undef glUniformMatrix2x4dv
	static inline void glUniformMatrix2x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glUniformMatrix2x4dv(location, count, transpose, value);
	}
#endif // glUniformMatrix2x4dv

#ifdef glUniformMatrix2x4fv
#undef glUniformMatrix2x4fv
	static inline void glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix2x4fv(location, count, transpose, value);
	}
#endif // glUniformMatrix2x4fv

#ifdef glUniformMatrix2x4fvNV
#undef glUniformMatrix2x4fvNV
	static inline void glUniformMatrix2x4fvNV(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix2x4fvNV(location, count, transpose, value);
	}
#endif // glUniformMatrix2x4fvNV

#ifdef glUniformMatrix3dv
#undef glUniformMatrix3dv
	static inline void glUniformMatrix3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glUniformMatrix3dv(location, count, transpose, value);
	}
#endif // glUniformMatrix3dv

#ifdef glUniformMatrix3fv
#undef glUniformMatrix3fv
	static inline void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix3fv(location, count, transpose, value);
	}
#endif // glUniformMatrix3fv

#ifdef glUniformMatrix3fvARB
#undef glUniformMatrix3fvARB
	static inline void glUniformMatrix3fvARB(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix3fvARB(location, count, transpose, value);
	}
#endif // glUniformMatrix3fvARB

#ifdef glUniformMatrix3x2dv
#undef glUniformMatrix3x2dv
	static inline void glUniformMatrix3x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glUniformMatrix3x2dv(location, count, transpose, value);
	}
#endif // glUniformMatrix3x2dv

#ifdef glUniformMatrix3x2fv
#undef glUniformMatrix3x2fv
	static inline void glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix3x2fv(location, count, transpose, value);
	}
#endif // glUniformMatrix3x2fv

#ifdef glUniformMatrix3x2fvNV
#undef glUniformMatrix3x2fvNV
	static inline void glUniformMatrix3x2fvNV(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix3x2fvNV(location, count, transpose, value);
	}
#endif // glUniformMatrix3x2fvNV

#ifdef glUniformMatrix3x4dv
#undef glUniformMatrix3x4dv
	static inline void glUniformMatrix3x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glUniformMatrix3x4dv(location, count, transpose, value);
	}
#endif // glUniformMatrix3x4dv

#ifdef glUniformMatrix3x4fv
#undef glUniformMatrix3x4fv
	static inline void glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix3x4fv(location, count, transpose, value);
	}
#endif // glUniformMatrix3x4fv

#ifdef glUniformMatrix3x4fvNV
#undef glUniformMatrix3x4fvNV
	static inline void glUniformMatrix3x4fvNV(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix3x4fvNV(location, count, transpose, value);
	}
#endif // glUniformMatrix3x4fvNV

#ifdef glUniformMatrix4dv
#undef glUniformMatrix4dv
	static inline void glUniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glUniformMatrix4dv(location, count, transpose, value);
	}
#endif // glUniformMatrix4dv

#ifdef glUniformMatrix4fv
#undef glUniformMatrix4fv
	static inline void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix4fv(location, count, transpose, value);
	}
#endif // glUniformMatrix4fv

#ifdef glUniformMatrix4fvARB
#undef glUniformMatrix4fvARB
	static inline void glUniformMatrix4fvARB(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix4fvARB(location, count, transpose, value);
	}
#endif // glUniformMatrix4fvARB

#ifdef glUniformMatrix4x2dv
#undef glUniformMatrix4x2dv
	static inline void glUniformMatrix4x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glUniformMatrix4x2dv(location, count, transpose, value);
	}
#endif // glUniformMatrix4x2dv

#ifdef glUniformMatrix4x2fv
#undef glUniformMatrix4x2fv
	static inline void glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix4x2fv(location, count, transpose, value);
	}
#endif // glUniformMatrix4x2fv

#ifdef glUniformMatrix4x2fvNV
#undef glUniformMatrix4x2fvNV
	static inline void glUniformMatrix4x2fvNV(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix4x2fvNV(location, count, transpose, value);
	}
#endif // glUniformMatrix4x2fvNV

#ifdef glUniformMatrix4x3dv
#undef glUniformMatrix4x3dv
	static inline void glUniformMatrix4x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value){
	    glad_debug_glUniformMatrix4x3dv(location, count, transpose, value);
	}
#endif // glUniformMatrix4x3dv

#ifdef glUniformMatrix4x3fv
#undef glUniformMatrix4x3fv
	static inline void glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix4x3fv(location, count, transpose, value);
	}
#endif // glUniformMatrix4x3fv

#ifdef glUniformMatrix4x3fvNV
#undef glUniformMatrix4x3fvNV
	static inline void glUniformMatrix4x3fvNV(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	    glad_debug_glUniformMatrix4x3fvNV(location, count, transpose, value);
	}
#endif // glUniformMatrix4x3fvNV

#ifdef glUniformSubroutinesuiv
#undef glUniformSubroutinesuiv
	static inline void glUniformSubroutinesuiv(GLenum shadertype, GLsizei count, const GLuint *indices){
	    glad_debug_glUniformSubroutinesuiv(shadertype, count, indices);
	}
#endif // glUniformSubroutinesuiv

#ifdef glUniformui64NV
#undef glUniformui64NV
	static inline void glUniformui64NV(GLint location, GLuint64EXT value){
	    glad_debug_glUniformui64NV(location, value);
	}
#endif // glUniformui64NV

#ifdef glUniformui64vNV
#undef glUniformui64vNV
	static inline void glUniformui64vNV(GLint location, GLsizei count, const GLuint64EXT *value){
	    glad_debug_glUniformui64vNV(location, count, value);
	}
#endif // glUniformui64vNV

#ifdef glUnlockArraysEXT
#undef glUnlockArraysEXT
	static inline void glUnlockArraysEXT(){
	    glad_debug_glUnlockArraysEXT();
	}
#endif // glUnlockArraysEXT

#ifdef glUnmapObjectBufferATI
#undef glUnmapObjectBufferATI
	static inline void glUnmapObjectBufferATI(GLuint buffer){
	    glad_debug_glUnmapObjectBufferATI(buffer);
	}
#endif // glUnmapObjectBufferATI

#ifdef glUnmapTexture2DINTEL
#undef glUnmapTexture2DINTEL
	static inline void glUnmapTexture2DINTEL(GLuint texture, GLint level){
	    glad_debug_glUnmapTexture2DINTEL(texture, level);
	}
#endif // glUnmapTexture2DINTEL

#ifdef glUploadGpuMaskNVX
#undef glUploadGpuMaskNVX
	static inline void glUploadGpuMaskNVX(GLbitfield mask){
	    glad_debug_glUploadGpuMaskNVX(mask);
	}
#endif // glUploadGpuMaskNVX

#ifdef glUseProgram
#undef glUseProgram
	static inline void glUseProgram(GLuint program){
	    glad_debug_glUseProgram(program);
	}
#endif // glUseProgram

#ifdef glUseProgramObjectARB
#undef glUseProgramObjectARB
	static inline void glUseProgramObjectARB(GLhandleARB programObj){
	    glad_debug_glUseProgramObjectARB(programObj);
	}
#endif // glUseProgramObjectARB

#ifdef glUseProgramStages
#undef glUseProgramStages
	static inline void glUseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program){
	    glad_debug_glUseProgramStages(pipeline, stages, program);
	}
#endif // glUseProgramStages

#ifdef glUseProgramStagesEXT
#undef glUseProgramStagesEXT
	static inline void glUseProgramStagesEXT(GLuint pipeline, GLbitfield stages, GLuint program){
	    glad_debug_glUseProgramStagesEXT(pipeline, stages, program);
	}
#endif // glUseProgramStagesEXT

#ifdef glUseShaderProgramEXT
#undef glUseShaderProgramEXT
	static inline void glUseShaderProgramEXT(GLenum type, GLuint program){
	    glad_debug_glUseShaderProgramEXT(type, program);
	}
#endif // glUseShaderProgramEXT

#ifdef glVDPAUFiniNV
#undef glVDPAUFiniNV
	static inline void glVDPAUFiniNV(){
	    glad_debug_glVDPAUFiniNV();
	}
#endif // glVDPAUFiniNV

#ifdef glVDPAUGetSurfaceivNV
#undef glVDPAUGetSurfaceivNV
	static inline void glVDPAUGetSurfaceivNV(GLvdpauSurfaceNV surface, GLenum pname, GLsizei count, GLsizei *length, GLint *values){
	    glad_debug_glVDPAUGetSurfaceivNV(surface, pname, count, length, values);
	}
#endif // glVDPAUGetSurfaceivNV

#ifdef glVDPAUMapSurfacesNV
#undef glVDPAUMapSurfacesNV
	static inline void glVDPAUMapSurfacesNV(GLsizei numSurfaces, const GLvdpauSurfaceNV *surfaces){
	    glad_debug_glVDPAUMapSurfacesNV(numSurfaces, surfaces);
	}
#endif // glVDPAUMapSurfacesNV

#ifdef glVDPAUSurfaceAccessNV
#undef glVDPAUSurfaceAccessNV
	static inline void glVDPAUSurfaceAccessNV(GLvdpauSurfaceNV surface, GLenum access){
	    glad_debug_glVDPAUSurfaceAccessNV(surface, access);
	}
#endif // glVDPAUSurfaceAccessNV

#ifdef glVDPAUUnmapSurfacesNV
#undef glVDPAUUnmapSurfacesNV
	static inline void glVDPAUUnmapSurfacesNV(GLsizei numSurface, const GLvdpauSurfaceNV *surfaces){
	    glad_debug_glVDPAUUnmapSurfacesNV(numSurface, surfaces);
	}
#endif // glVDPAUUnmapSurfacesNV

#ifdef glVDPAUUnregisterSurfaceNV
#undef glVDPAUUnregisterSurfaceNV
	static inline void glVDPAUUnregisterSurfaceNV(GLvdpauSurfaceNV surface){
	    glad_debug_glVDPAUUnregisterSurfaceNV(surface);
	}
#endif // glVDPAUUnregisterSurfaceNV

#ifdef glValidateProgram
#undef glValidateProgram
	static inline void glValidateProgram(GLuint program){
	    glad_debug_glValidateProgram(program);
	}
#endif // glValidateProgram

#ifdef glValidateProgramARB
#undef glValidateProgramARB
	static inline void glValidateProgramARB(GLhandleARB programObj){
	    glad_debug_glValidateProgramARB(programObj);
	}
#endif // glValidateProgramARB

#ifdef glValidateProgramPipeline
#undef glValidateProgramPipeline
	static inline void glValidateProgramPipeline(GLuint pipeline){
	    glad_debug_glValidateProgramPipeline(pipeline);
	}
#endif // glValidateProgramPipeline

#ifdef glValidateProgramPipelineEXT
#undef glValidateProgramPipelineEXT
	static inline void glValidateProgramPipelineEXT(GLuint pipeline){
	    glad_debug_glValidateProgramPipelineEXT(pipeline);
	}
#endif // glValidateProgramPipelineEXT

#ifdef glVariantArrayObjectATI
#undef glVariantArrayObjectATI
	static inline void glVariantArrayObjectATI(GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset){
	    glad_debug_glVariantArrayObjectATI(id, type, stride, buffer, offset);
	}
#endif // glVariantArrayObjectATI

#ifdef glVariantbvEXT
#undef glVariantbvEXT
	static inline void glVariantbvEXT(GLuint id, const GLbyte *addr){
	    glad_debug_glVariantbvEXT(id, addr);
	}
#endif // glVariantbvEXT

#ifdef glVariantdvEXT
#undef glVariantdvEXT
	static inline void glVariantdvEXT(GLuint id, const GLdouble *addr){
	    glad_debug_glVariantdvEXT(id, addr);
	}
#endif // glVariantdvEXT

#ifdef glVariantfvEXT
#undef glVariantfvEXT
	static inline void glVariantfvEXT(GLuint id, const GLfloat *addr){
	    glad_debug_glVariantfvEXT(id, addr);
	}
#endif // glVariantfvEXT

#ifdef glVariantivEXT
#undef glVariantivEXT
	static inline void glVariantivEXT(GLuint id, const GLint *addr){
	    glad_debug_glVariantivEXT(id, addr);
	}
#endif // glVariantivEXT

#ifdef glVariantsvEXT
#undef glVariantsvEXT
	static inline void glVariantsvEXT(GLuint id, const GLshort *addr){
	    glad_debug_glVariantsvEXT(id, addr);
	}
#endif // glVariantsvEXT

#ifdef glVariantubvEXT
#undef glVariantubvEXT
	static inline void glVariantubvEXT(GLuint id, const GLubyte *addr){
	    glad_debug_glVariantubvEXT(id, addr);
	}
#endif // glVariantubvEXT

#ifdef glVariantuivEXT
#undef glVariantuivEXT
	static inline void glVariantuivEXT(GLuint id, const GLuint *addr){
	    glad_debug_glVariantuivEXT(id, addr);
	}
#endif // glVariantuivEXT

#ifdef glVariantusvEXT
#undef glVariantusvEXT
	static inline void glVariantusvEXT(GLuint id, const GLushort *addr){
	    glad_debug_glVariantusvEXT(id, addr);
	}
#endif // glVariantusvEXT

#ifdef glVertex2bOES
#undef glVertex2bOES
	static inline void glVertex2bOES(GLbyte x, GLbyte y){
	    glad_debug_glVertex2bOES(x, y);
	}
#endif // glVertex2bOES

#ifdef glVertex2bvOES
#undef glVertex2bvOES
	static inline void glVertex2bvOES(const GLbyte *coords){
	    glad_debug_glVertex2bvOES(coords);
	}
#endif // glVertex2bvOES

#ifdef glVertex2d
#undef glVertex2d
	static inline void glVertex2d(GLdouble x, GLdouble y){
	    glad_debug_glVertex2d(x, y);
	}
#endif // glVertex2d

#ifdef glVertex2dv
#undef glVertex2dv
	static inline void glVertex2dv(const GLdouble *v){
	    glad_debug_glVertex2dv(v);
	}
#endif // glVertex2dv

#ifdef glVertex2f
#undef glVertex2f
	static inline void glVertex2f(GLfloat x, GLfloat y){
	    glad_debug_glVertex2f(x, y);
	}
#endif // glVertex2f

#ifdef glVertex2fv
#undef glVertex2fv
	static inline void glVertex2fv(const GLfloat *v){
	    glad_debug_glVertex2fv(v);
	}
#endif // glVertex2fv

#ifdef glVertex2hNV
#undef glVertex2hNV
	static inline void glVertex2hNV(GLhalfNV x, GLhalfNV y){
	    glad_debug_glVertex2hNV(x, y);
	}
#endif // glVertex2hNV

#ifdef glVertex2hvNV
#undef glVertex2hvNV
	static inline void glVertex2hvNV(const GLhalfNV *v){
	    glad_debug_glVertex2hvNV(v);
	}
#endif // glVertex2hvNV

#ifdef glVertex2i
#undef glVertex2i
	static inline void glVertex2i(GLint x, GLint y){
	    glad_debug_glVertex2i(x, y);
	}
#endif // glVertex2i

#ifdef glVertex2iv
#undef glVertex2iv
	static inline void glVertex2iv(const GLint *v){
	    glad_debug_glVertex2iv(v);
	}
#endif // glVertex2iv

#ifdef glVertex2s
#undef glVertex2s
	static inline void glVertex2s(GLshort x, GLshort y){
	    glad_debug_glVertex2s(x, y);
	}
#endif // glVertex2s

#ifdef glVertex2sv
#undef glVertex2sv
	static inline void glVertex2sv(const GLshort *v){
	    glad_debug_glVertex2sv(v);
	}
#endif // glVertex2sv

#ifdef glVertex2xOES
#undef glVertex2xOES
	static inline void glVertex2xOES(GLfixed x){
	    glad_debug_glVertex2xOES(x);
	}
#endif // glVertex2xOES

#ifdef glVertex2xvOES
#undef glVertex2xvOES
	static inline void glVertex2xvOES(const GLfixed *coords){
	    glad_debug_glVertex2xvOES(coords);
	}
#endif // glVertex2xvOES

#ifdef glVertex3bOES
#undef glVertex3bOES
	static inline void glVertex3bOES(GLbyte x, GLbyte y, GLbyte z){
	    glad_debug_glVertex3bOES(x, y, z);
	}
#endif // glVertex3bOES

#ifdef glVertex3bvOES
#undef glVertex3bvOES
	static inline void glVertex3bvOES(const GLbyte *coords){
	    glad_debug_glVertex3bvOES(coords);
	}
#endif // glVertex3bvOES

#ifdef glVertex3d
#undef glVertex3d
	static inline void glVertex3d(GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glVertex3d(x, y, z);
	}
#endif // glVertex3d

#ifdef glVertex3dv
#undef glVertex3dv
	static inline void glVertex3dv(const GLdouble *v){
	    glad_debug_glVertex3dv(v);
	}
#endif // glVertex3dv

#ifdef glVertex3f
#undef glVertex3f
	static inline void glVertex3f(GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glVertex3f(x, y, z);
	}
#endif // glVertex3f

#ifdef glVertex3fv
#undef glVertex3fv
	static inline void glVertex3fv(const GLfloat *v){
	    glad_debug_glVertex3fv(v);
	}
#endif // glVertex3fv

#ifdef glVertex3hNV
#undef glVertex3hNV
	static inline void glVertex3hNV(GLhalfNV x, GLhalfNV y, GLhalfNV z){
	    glad_debug_glVertex3hNV(x, y, z);
	}
#endif // glVertex3hNV

#ifdef glVertex3hvNV
#undef glVertex3hvNV
	static inline void glVertex3hvNV(const GLhalfNV *v){
	    glad_debug_glVertex3hvNV(v);
	}
#endif // glVertex3hvNV

#ifdef glVertex3i
#undef glVertex3i
	static inline void glVertex3i(GLint x, GLint y, GLint z){
	    glad_debug_glVertex3i(x, y, z);
	}
#endif // glVertex3i

#ifdef glVertex3iv
#undef glVertex3iv
	static inline void glVertex3iv(const GLint *v){
	    glad_debug_glVertex3iv(v);
	}
#endif // glVertex3iv

#ifdef glVertex3s
#undef glVertex3s
	static inline void glVertex3s(GLshort x, GLshort y, GLshort z){
	    glad_debug_glVertex3s(x, y, z);
	}
#endif // glVertex3s

#ifdef glVertex3sv
#undef glVertex3sv
	static inline void glVertex3sv(const GLshort *v){
	    glad_debug_glVertex3sv(v);
	}
#endif // glVertex3sv

#ifdef glVertex3xOES
#undef glVertex3xOES
	static inline void glVertex3xOES(GLfixed x, GLfixed y){
	    glad_debug_glVertex3xOES(x, y);
	}
#endif // glVertex3xOES

#ifdef glVertex3xvOES
#undef glVertex3xvOES
	static inline void glVertex3xvOES(const GLfixed *coords){
	    glad_debug_glVertex3xvOES(coords);
	}
#endif // glVertex3xvOES

#ifdef glVertex4bOES
#undef glVertex4bOES
	static inline void glVertex4bOES(GLbyte x, GLbyte y, GLbyte z, GLbyte w){
	    glad_debug_glVertex4bOES(x, y, z, w);
	}
#endif // glVertex4bOES

#ifdef glVertex4bvOES
#undef glVertex4bvOES
	static inline void glVertex4bvOES(const GLbyte *coords){
	    glad_debug_glVertex4bvOES(coords);
	}
#endif // glVertex4bvOES

#ifdef glVertex4d
#undef glVertex4d
	static inline void glVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glVertex4d(x, y, z, w);
	}
#endif // glVertex4d

#ifdef glVertex4dv
#undef glVertex4dv
	static inline void glVertex4dv(const GLdouble *v){
	    glad_debug_glVertex4dv(v);
	}
#endif // glVertex4dv

#ifdef glVertex4f
#undef glVertex4f
	static inline void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glVertex4f(x, y, z, w);
	}
#endif // glVertex4f

#ifdef glVertex4fv
#undef glVertex4fv
	static inline void glVertex4fv(const GLfloat *v){
	    glad_debug_glVertex4fv(v);
	}
#endif // glVertex4fv

#ifdef glVertex4hNV
#undef glVertex4hNV
	static inline void glVertex4hNV(GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w){
	    glad_debug_glVertex4hNV(x, y, z, w);
	}
#endif // glVertex4hNV

#ifdef glVertex4hvNV
#undef glVertex4hvNV
	static inline void glVertex4hvNV(const GLhalfNV *v){
	    glad_debug_glVertex4hvNV(v);
	}
#endif // glVertex4hvNV

#ifdef glVertex4i
#undef glVertex4i
	static inline void glVertex4i(GLint x, GLint y, GLint z, GLint w){
	    glad_debug_glVertex4i(x, y, z, w);
	}
#endif // glVertex4i

#ifdef glVertex4iv
#undef glVertex4iv
	static inline void glVertex4iv(const GLint *v){
	    glad_debug_glVertex4iv(v);
	}
#endif // glVertex4iv

#ifdef glVertex4s
#undef glVertex4s
	static inline void glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w){
	    glad_debug_glVertex4s(x, y, z, w);
	}
#endif // glVertex4s

#ifdef glVertex4sv
#undef glVertex4sv
	static inline void glVertex4sv(const GLshort *v){
	    glad_debug_glVertex4sv(v);
	}
#endif // glVertex4sv

#ifdef glVertex4xOES
#undef glVertex4xOES
	static inline void glVertex4xOES(GLfixed x, GLfixed y, GLfixed z){
	    glad_debug_glVertex4xOES(x, y, z);
	}
#endif // glVertex4xOES

#ifdef glVertex4xvOES
#undef glVertex4xvOES
	static inline void glVertex4xvOES(const GLfixed *coords){
	    glad_debug_glVertex4xvOES(coords);
	}
#endif // glVertex4xvOES

#ifdef glVertexArrayAttribBinding
#undef glVertexArrayAttribBinding
	static inline void glVertexArrayAttribBinding(GLuint vaobj, GLuint attribindex, GLuint bindingindex){
	    glad_debug_glVertexArrayAttribBinding(vaobj, attribindex, bindingindex);
	}
#endif // glVertexArrayAttribBinding

#ifdef glVertexArrayAttribFormat
#undef glVertexArrayAttribFormat
	static inline void glVertexArrayAttribFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset){
	    glad_debug_glVertexArrayAttribFormat(vaobj, attribindex, size, type, normalized, relativeoffset);
	}
#endif // glVertexArrayAttribFormat

#ifdef glVertexArrayAttribIFormat
#undef glVertexArrayAttribIFormat
	static inline void glVertexArrayAttribIFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset){
	    glad_debug_glVertexArrayAttribIFormat(vaobj, attribindex, size, type, relativeoffset);
	}
#endif // glVertexArrayAttribIFormat

#ifdef glVertexArrayAttribLFormat
#undef glVertexArrayAttribLFormat
	static inline void glVertexArrayAttribLFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset){
	    glad_debug_glVertexArrayAttribLFormat(vaobj, attribindex, size, type, relativeoffset);
	}
#endif // glVertexArrayAttribLFormat

#ifdef glVertexArrayBindVertexBufferEXT
#undef glVertexArrayBindVertexBufferEXT
	static inline void glVertexArrayBindVertexBufferEXT(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride){
	    glad_debug_glVertexArrayBindVertexBufferEXT(vaobj, bindingindex, buffer, offset, stride);
	}
#endif // glVertexArrayBindVertexBufferEXT

#ifdef glVertexArrayBindingDivisor
#undef glVertexArrayBindingDivisor
	static inline void glVertexArrayBindingDivisor(GLuint vaobj, GLuint bindingindex, GLuint divisor){
	    glad_debug_glVertexArrayBindingDivisor(vaobj, bindingindex, divisor);
	}
#endif // glVertexArrayBindingDivisor

#ifdef glVertexArrayColorOffsetEXT
#undef glVertexArrayColorOffsetEXT
	static inline void glVertexArrayColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset){
	    glad_debug_glVertexArrayColorOffsetEXT(vaobj, buffer, size, type, stride, offset);
	}
#endif // glVertexArrayColorOffsetEXT

#ifdef glVertexArrayEdgeFlagOffsetEXT
#undef glVertexArrayEdgeFlagOffsetEXT
	static inline void glVertexArrayEdgeFlagOffsetEXT(GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset){
	    glad_debug_glVertexArrayEdgeFlagOffsetEXT(vaobj, buffer, stride, offset);
	}
#endif // glVertexArrayEdgeFlagOffsetEXT

#ifdef glVertexArrayElementBuffer
#undef glVertexArrayElementBuffer
	static inline void glVertexArrayElementBuffer(GLuint vaobj, GLuint buffer){
	    glad_debug_glVertexArrayElementBuffer(vaobj, buffer);
	}
#endif // glVertexArrayElementBuffer

#ifdef glVertexArrayFogCoordOffsetEXT
#undef glVertexArrayFogCoordOffsetEXT
	static inline void glVertexArrayFogCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset){
	    glad_debug_glVertexArrayFogCoordOffsetEXT(vaobj, buffer, type, stride, offset);
	}
#endif // glVertexArrayFogCoordOffsetEXT

#ifdef glVertexArrayIndexOffsetEXT
#undef glVertexArrayIndexOffsetEXT
	static inline void glVertexArrayIndexOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset){
	    glad_debug_glVertexArrayIndexOffsetEXT(vaobj, buffer, type, stride, offset);
	}
#endif // glVertexArrayIndexOffsetEXT

#ifdef glVertexArrayMultiTexCoordOffsetEXT
#undef glVertexArrayMultiTexCoordOffsetEXT
	static inline void glVertexArrayMultiTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset){
	    glad_debug_glVertexArrayMultiTexCoordOffsetEXT(vaobj, buffer, texunit, size, type, stride, offset);
	}
#endif // glVertexArrayMultiTexCoordOffsetEXT

#ifdef glVertexArrayNormalOffsetEXT
#undef glVertexArrayNormalOffsetEXT
	static inline void glVertexArrayNormalOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset){
	    glad_debug_glVertexArrayNormalOffsetEXT(vaobj, buffer, type, stride, offset);
	}
#endif // glVertexArrayNormalOffsetEXT

#ifdef glVertexArrayParameteriAPPLE
#undef glVertexArrayParameteriAPPLE
	static inline void glVertexArrayParameteriAPPLE(GLenum pname, GLint param){
	    glad_debug_glVertexArrayParameteriAPPLE(pname, param);
	}
#endif // glVertexArrayParameteriAPPLE

#ifdef glVertexArraySecondaryColorOffsetEXT
#undef glVertexArraySecondaryColorOffsetEXT
	static inline void glVertexArraySecondaryColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset){
	    glad_debug_glVertexArraySecondaryColorOffsetEXT(vaobj, buffer, size, type, stride, offset);
	}
#endif // glVertexArraySecondaryColorOffsetEXT

#ifdef glVertexArrayTexCoordOffsetEXT
#undef glVertexArrayTexCoordOffsetEXT
	static inline void glVertexArrayTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset){
	    glad_debug_glVertexArrayTexCoordOffsetEXT(vaobj, buffer, size, type, stride, offset);
	}
#endif // glVertexArrayTexCoordOffsetEXT

#ifdef glVertexArrayVertexAttribBindingEXT
#undef glVertexArrayVertexAttribBindingEXT
	static inline void glVertexArrayVertexAttribBindingEXT(GLuint vaobj, GLuint attribindex, GLuint bindingindex){
	    glad_debug_glVertexArrayVertexAttribBindingEXT(vaobj, attribindex, bindingindex);
	}
#endif // glVertexArrayVertexAttribBindingEXT

#ifdef glVertexArrayVertexAttribDivisorEXT
#undef glVertexArrayVertexAttribDivisorEXT
	static inline void glVertexArrayVertexAttribDivisorEXT(GLuint vaobj, GLuint index, GLuint divisor){
	    glad_debug_glVertexArrayVertexAttribDivisorEXT(vaobj, index, divisor);
	}
#endif // glVertexArrayVertexAttribDivisorEXT

#ifdef glVertexArrayVertexAttribFormatEXT
#undef glVertexArrayVertexAttribFormatEXT
	static inline void glVertexArrayVertexAttribFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset){
	    glad_debug_glVertexArrayVertexAttribFormatEXT(vaobj, attribindex, size, type, normalized, relativeoffset);
	}
#endif // glVertexArrayVertexAttribFormatEXT

#ifdef glVertexArrayVertexAttribIFormatEXT
#undef glVertexArrayVertexAttribIFormatEXT
	static inline void glVertexArrayVertexAttribIFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset){
	    glad_debug_glVertexArrayVertexAttribIFormatEXT(vaobj, attribindex, size, type, relativeoffset);
	}
#endif // glVertexArrayVertexAttribIFormatEXT

#ifdef glVertexArrayVertexAttribIOffsetEXT
#undef glVertexArrayVertexAttribIOffsetEXT
	static inline void glVertexArrayVertexAttribIOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset){
	    glad_debug_glVertexArrayVertexAttribIOffsetEXT(vaobj, buffer, index, size, type, stride, offset);
	}
#endif // glVertexArrayVertexAttribIOffsetEXT

#ifdef glVertexArrayVertexAttribLFormatEXT
#undef glVertexArrayVertexAttribLFormatEXT
	static inline void glVertexArrayVertexAttribLFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset){
	    glad_debug_glVertexArrayVertexAttribLFormatEXT(vaobj, attribindex, size, type, relativeoffset);
	}
#endif // glVertexArrayVertexAttribLFormatEXT

#ifdef glVertexArrayVertexAttribLOffsetEXT
#undef glVertexArrayVertexAttribLOffsetEXT
	static inline void glVertexArrayVertexAttribLOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset){
	    glad_debug_glVertexArrayVertexAttribLOffsetEXT(vaobj, buffer, index, size, type, stride, offset);
	}
#endif // glVertexArrayVertexAttribLOffsetEXT

#ifdef glVertexArrayVertexAttribOffsetEXT
#undef glVertexArrayVertexAttribOffsetEXT
	static inline void glVertexArrayVertexAttribOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset){
	    glad_debug_glVertexArrayVertexAttribOffsetEXT(vaobj, buffer, index, size, type, normalized, stride, offset);
	}
#endif // glVertexArrayVertexAttribOffsetEXT

#ifdef glVertexArrayVertexBindingDivisorEXT
#undef glVertexArrayVertexBindingDivisorEXT
	static inline void glVertexArrayVertexBindingDivisorEXT(GLuint vaobj, GLuint bindingindex, GLuint divisor){
	    glad_debug_glVertexArrayVertexBindingDivisorEXT(vaobj, bindingindex, divisor);
	}
#endif // glVertexArrayVertexBindingDivisorEXT

#ifdef glVertexArrayVertexBuffer
#undef glVertexArrayVertexBuffer
	static inline void glVertexArrayVertexBuffer(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride){
	    glad_debug_glVertexArrayVertexBuffer(vaobj, bindingindex, buffer, offset, stride);
	}
#endif // glVertexArrayVertexBuffer

#ifdef glVertexArrayVertexBuffers
#undef glVertexArrayVertexBuffers
	static inline void glVertexArrayVertexBuffers(GLuint vaobj, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides){
	    glad_debug_glVertexArrayVertexBuffers(vaobj, first, count, buffers, offsets, strides);
	}
#endif // glVertexArrayVertexBuffers

#ifdef glVertexArrayVertexOffsetEXT
#undef glVertexArrayVertexOffsetEXT
	static inline void glVertexArrayVertexOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset){
	    glad_debug_glVertexArrayVertexOffsetEXT(vaobj, buffer, size, type, stride, offset);
	}
#endif // glVertexArrayVertexOffsetEXT

#ifdef glVertexAttrib1d
#undef glVertexAttrib1d
	static inline void glVertexAttrib1d(GLuint index, GLdouble x){
	    glad_debug_glVertexAttrib1d(index, x);
	}
#endif // glVertexAttrib1d

#ifdef glVertexAttrib1dARB
#undef glVertexAttrib1dARB
	static inline void glVertexAttrib1dARB(GLuint index, GLdouble x){
	    glad_debug_glVertexAttrib1dARB(index, x);
	}
#endif // glVertexAttrib1dARB

#ifdef glVertexAttrib1dNV
#undef glVertexAttrib1dNV
	static inline void glVertexAttrib1dNV(GLuint index, GLdouble x){
	    glad_debug_glVertexAttrib1dNV(index, x);
	}
#endif // glVertexAttrib1dNV

#ifdef glVertexAttrib1dv
#undef glVertexAttrib1dv
	static inline void glVertexAttrib1dv(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttrib1dv(index, v);
	}
#endif // glVertexAttrib1dv

#ifdef glVertexAttrib1dvARB
#undef glVertexAttrib1dvARB
	static inline void glVertexAttrib1dvARB(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttrib1dvARB(index, v);
	}
#endif // glVertexAttrib1dvARB

#ifdef glVertexAttrib1dvNV
#undef glVertexAttrib1dvNV
	static inline void glVertexAttrib1dvNV(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttrib1dvNV(index, v);
	}
#endif // glVertexAttrib1dvNV

#ifdef glVertexAttrib1f
#undef glVertexAttrib1f
	static inline void glVertexAttrib1f(GLuint index, GLfloat x){
	    glad_debug_glVertexAttrib1f(index, x);
	}
#endif // glVertexAttrib1f

#ifdef glVertexAttrib1fARB
#undef glVertexAttrib1fARB
	static inline void glVertexAttrib1fARB(GLuint index, GLfloat x){
	    glad_debug_glVertexAttrib1fARB(index, x);
	}
#endif // glVertexAttrib1fARB

#ifdef glVertexAttrib1fNV
#undef glVertexAttrib1fNV
	static inline void glVertexAttrib1fNV(GLuint index, GLfloat x){
	    glad_debug_glVertexAttrib1fNV(index, x);
	}
#endif // glVertexAttrib1fNV

#ifdef glVertexAttrib1fv
#undef glVertexAttrib1fv
	static inline void glVertexAttrib1fv(GLuint index, const GLfloat *v){
	    glad_debug_glVertexAttrib1fv(index, v);
	}
#endif // glVertexAttrib1fv

#ifdef glVertexAttrib1fvARB
#undef glVertexAttrib1fvARB
	static inline void glVertexAttrib1fvARB(GLuint index, const GLfloat *v){
	    glad_debug_glVertexAttrib1fvARB(index, v);
	}
#endif // glVertexAttrib1fvARB

#ifdef glVertexAttrib1fvNV
#undef glVertexAttrib1fvNV
	static inline void glVertexAttrib1fvNV(GLuint index, const GLfloat *v){
	    glad_debug_glVertexAttrib1fvNV(index, v);
	}
#endif // glVertexAttrib1fvNV

#ifdef glVertexAttrib1hNV
#undef glVertexAttrib1hNV
	static inline void glVertexAttrib1hNV(GLuint index, GLhalfNV x){
	    glad_debug_glVertexAttrib1hNV(index, x);
	}
#endif // glVertexAttrib1hNV

#ifdef glVertexAttrib1hvNV
#undef glVertexAttrib1hvNV
	static inline void glVertexAttrib1hvNV(GLuint index, const GLhalfNV *v){
	    glad_debug_glVertexAttrib1hvNV(index, v);
	}
#endif // glVertexAttrib1hvNV

#ifdef glVertexAttrib1s
#undef glVertexAttrib1s
	static inline void glVertexAttrib1s(GLuint index, GLshort x){
	    glad_debug_glVertexAttrib1s(index, x);
	}
#endif // glVertexAttrib1s

#ifdef glVertexAttrib1sARB
#undef glVertexAttrib1sARB
	static inline void glVertexAttrib1sARB(GLuint index, GLshort x){
	    glad_debug_glVertexAttrib1sARB(index, x);
	}
#endif // glVertexAttrib1sARB

#ifdef glVertexAttrib1sNV
#undef glVertexAttrib1sNV
	static inline void glVertexAttrib1sNV(GLuint index, GLshort x){
	    glad_debug_glVertexAttrib1sNV(index, x);
	}
#endif // glVertexAttrib1sNV

#ifdef glVertexAttrib1sv
#undef glVertexAttrib1sv
	static inline void glVertexAttrib1sv(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib1sv(index, v);
	}
#endif // glVertexAttrib1sv

#ifdef glVertexAttrib1svARB
#undef glVertexAttrib1svARB
	static inline void glVertexAttrib1svARB(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib1svARB(index, v);
	}
#endif // glVertexAttrib1svARB

#ifdef glVertexAttrib1svNV
#undef glVertexAttrib1svNV
	static inline void glVertexAttrib1svNV(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib1svNV(index, v);
	}
#endif // glVertexAttrib1svNV

#ifdef glVertexAttrib2d
#undef glVertexAttrib2d
	static inline void glVertexAttrib2d(GLuint index, GLdouble x, GLdouble y){
	    glad_debug_glVertexAttrib2d(index, x, y);
	}
#endif // glVertexAttrib2d

#ifdef glVertexAttrib2dARB
#undef glVertexAttrib2dARB
	static inline void glVertexAttrib2dARB(GLuint index, GLdouble x, GLdouble y){
	    glad_debug_glVertexAttrib2dARB(index, x, y);
	}
#endif // glVertexAttrib2dARB

#ifdef glVertexAttrib2dNV
#undef glVertexAttrib2dNV
	static inline void glVertexAttrib2dNV(GLuint index, GLdouble x, GLdouble y){
	    glad_debug_glVertexAttrib2dNV(index, x, y);
	}
#endif // glVertexAttrib2dNV

#ifdef glVertexAttrib2dv
#undef glVertexAttrib2dv
	static inline void glVertexAttrib2dv(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttrib2dv(index, v);
	}
#endif // glVertexAttrib2dv

#ifdef glVertexAttrib2dvARB
#undef glVertexAttrib2dvARB
	static inline void glVertexAttrib2dvARB(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttrib2dvARB(index, v);
	}
#endif // glVertexAttrib2dvARB

#ifdef glVertexAttrib2dvNV
#undef glVertexAttrib2dvNV
	static inline void glVertexAttrib2dvNV(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttrib2dvNV(index, v);
	}
#endif // glVertexAttrib2dvNV

#ifdef glVertexAttrib2f
#undef glVertexAttrib2f
	static inline void glVertexAttrib2f(GLuint index, GLfloat x, GLfloat y){
	    glad_debug_glVertexAttrib2f(index, x, y);
	}
#endif // glVertexAttrib2f

#ifdef glVertexAttrib2fARB
#undef glVertexAttrib2fARB
	static inline void glVertexAttrib2fARB(GLuint index, GLfloat x, GLfloat y){
	    glad_debug_glVertexAttrib2fARB(index, x, y);
	}
#endif // glVertexAttrib2fARB

#ifdef glVertexAttrib2fNV
#undef glVertexAttrib2fNV
	static inline void glVertexAttrib2fNV(GLuint index, GLfloat x, GLfloat y){
	    glad_debug_glVertexAttrib2fNV(index, x, y);
	}
#endif // glVertexAttrib2fNV

#ifdef glVertexAttrib2fv
#undef glVertexAttrib2fv
	static inline void glVertexAttrib2fv(GLuint index, const GLfloat *v){
	    glad_debug_glVertexAttrib2fv(index, v);
	}
#endif // glVertexAttrib2fv

#ifdef glVertexAttrib2fvARB
#undef glVertexAttrib2fvARB
	static inline void glVertexAttrib2fvARB(GLuint index, const GLfloat *v){
	    glad_debug_glVertexAttrib2fvARB(index, v);
	}
#endif // glVertexAttrib2fvARB

#ifdef glVertexAttrib2fvNV
#undef glVertexAttrib2fvNV
	static inline void glVertexAttrib2fvNV(GLuint index, const GLfloat *v){
	    glad_debug_glVertexAttrib2fvNV(index, v);
	}
#endif // glVertexAttrib2fvNV

#ifdef glVertexAttrib2hNV
#undef glVertexAttrib2hNV
	static inline void glVertexAttrib2hNV(GLuint index, GLhalfNV x, GLhalfNV y){
	    glad_debug_glVertexAttrib2hNV(index, x, y);
	}
#endif // glVertexAttrib2hNV

#ifdef glVertexAttrib2hvNV
#undef glVertexAttrib2hvNV
	static inline void glVertexAttrib2hvNV(GLuint index, const GLhalfNV *v){
	    glad_debug_glVertexAttrib2hvNV(index, v);
	}
#endif // glVertexAttrib2hvNV

#ifdef glVertexAttrib2s
#undef glVertexAttrib2s
	static inline void glVertexAttrib2s(GLuint index, GLshort x, GLshort y){
	    glad_debug_glVertexAttrib2s(index, x, y);
	}
#endif // glVertexAttrib2s

#ifdef glVertexAttrib2sARB
#undef glVertexAttrib2sARB
	static inline void glVertexAttrib2sARB(GLuint index, GLshort x, GLshort y){
	    glad_debug_glVertexAttrib2sARB(index, x, y);
	}
#endif // glVertexAttrib2sARB

#ifdef glVertexAttrib2sNV
#undef glVertexAttrib2sNV
	static inline void glVertexAttrib2sNV(GLuint index, GLshort x, GLshort y){
	    glad_debug_glVertexAttrib2sNV(index, x, y);
	}
#endif // glVertexAttrib2sNV

#ifdef glVertexAttrib2sv
#undef glVertexAttrib2sv
	static inline void glVertexAttrib2sv(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib2sv(index, v);
	}
#endif // glVertexAttrib2sv

#ifdef glVertexAttrib2svARB
#undef glVertexAttrib2svARB
	static inline void glVertexAttrib2svARB(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib2svARB(index, v);
	}
#endif // glVertexAttrib2svARB

#ifdef glVertexAttrib2svNV
#undef glVertexAttrib2svNV
	static inline void glVertexAttrib2svNV(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib2svNV(index, v);
	}
#endif // glVertexAttrib2svNV

#ifdef glVertexAttrib3d
#undef glVertexAttrib3d
	static inline void glVertexAttrib3d(GLuint index, GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glVertexAttrib3d(index, x, y, z);
	}
#endif // glVertexAttrib3d

#ifdef glVertexAttrib3dARB
#undef glVertexAttrib3dARB
	static inline void glVertexAttrib3dARB(GLuint index, GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glVertexAttrib3dARB(index, x, y, z);
	}
#endif // glVertexAttrib3dARB

#ifdef glVertexAttrib3dNV
#undef glVertexAttrib3dNV
	static inline void glVertexAttrib3dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glVertexAttrib3dNV(index, x, y, z);
	}
#endif // glVertexAttrib3dNV

#ifdef glVertexAttrib3dv
#undef glVertexAttrib3dv
	static inline void glVertexAttrib3dv(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttrib3dv(index, v);
	}
#endif // glVertexAttrib3dv

#ifdef glVertexAttrib3dvARB
#undef glVertexAttrib3dvARB
	static inline void glVertexAttrib3dvARB(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttrib3dvARB(index, v);
	}
#endif // glVertexAttrib3dvARB

#ifdef glVertexAttrib3dvNV
#undef glVertexAttrib3dvNV
	static inline void glVertexAttrib3dvNV(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttrib3dvNV(index, v);
	}
#endif // glVertexAttrib3dvNV

#ifdef glVertexAttrib3f
#undef glVertexAttrib3f
	static inline void glVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glVertexAttrib3f(index, x, y, z);
	}
#endif // glVertexAttrib3f

#ifdef glVertexAttrib3fARB
#undef glVertexAttrib3fARB
	static inline void glVertexAttrib3fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glVertexAttrib3fARB(index, x, y, z);
	}
#endif // glVertexAttrib3fARB

#ifdef glVertexAttrib3fNV
#undef glVertexAttrib3fNV
	static inline void glVertexAttrib3fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glVertexAttrib3fNV(index, x, y, z);
	}
#endif // glVertexAttrib3fNV

#ifdef glVertexAttrib3fv
#undef glVertexAttrib3fv
	static inline void glVertexAttrib3fv(GLuint index, const GLfloat *v){
	    glad_debug_glVertexAttrib3fv(index, v);
	}
#endif // glVertexAttrib3fv

#ifdef glVertexAttrib3fvARB
#undef glVertexAttrib3fvARB
	static inline void glVertexAttrib3fvARB(GLuint index, const GLfloat *v){
	    glad_debug_glVertexAttrib3fvARB(index, v);
	}
#endif // glVertexAttrib3fvARB

#ifdef glVertexAttrib3fvNV
#undef glVertexAttrib3fvNV
	static inline void glVertexAttrib3fvNV(GLuint index, const GLfloat *v){
	    glad_debug_glVertexAttrib3fvNV(index, v);
	}
#endif // glVertexAttrib3fvNV

#ifdef glVertexAttrib3hNV
#undef glVertexAttrib3hNV
	static inline void glVertexAttrib3hNV(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z){
	    glad_debug_glVertexAttrib3hNV(index, x, y, z);
	}
#endif // glVertexAttrib3hNV

#ifdef glVertexAttrib3hvNV
#undef glVertexAttrib3hvNV
	static inline void glVertexAttrib3hvNV(GLuint index, const GLhalfNV *v){
	    glad_debug_glVertexAttrib3hvNV(index, v);
	}
#endif // glVertexAttrib3hvNV

#ifdef glVertexAttrib3s
#undef glVertexAttrib3s
	static inline void glVertexAttrib3s(GLuint index, GLshort x, GLshort y, GLshort z){
	    glad_debug_glVertexAttrib3s(index, x, y, z);
	}
#endif // glVertexAttrib3s

#ifdef glVertexAttrib3sARB
#undef glVertexAttrib3sARB
	static inline void glVertexAttrib3sARB(GLuint index, GLshort x, GLshort y, GLshort z){
	    glad_debug_glVertexAttrib3sARB(index, x, y, z);
	}
#endif // glVertexAttrib3sARB

#ifdef glVertexAttrib3sNV
#undef glVertexAttrib3sNV
	static inline void glVertexAttrib3sNV(GLuint index, GLshort x, GLshort y, GLshort z){
	    glad_debug_glVertexAttrib3sNV(index, x, y, z);
	}
#endif // glVertexAttrib3sNV

#ifdef glVertexAttrib3sv
#undef glVertexAttrib3sv
	static inline void glVertexAttrib3sv(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib3sv(index, v);
	}
#endif // glVertexAttrib3sv

#ifdef glVertexAttrib3svARB
#undef glVertexAttrib3svARB
	static inline void glVertexAttrib3svARB(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib3svARB(index, v);
	}
#endif // glVertexAttrib3svARB

#ifdef glVertexAttrib3svNV
#undef glVertexAttrib3svNV
	static inline void glVertexAttrib3svNV(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib3svNV(index, v);
	}
#endif // glVertexAttrib3svNV

#ifdef glVertexAttrib4Nbv
#undef glVertexAttrib4Nbv
	static inline void glVertexAttrib4Nbv(GLuint index, const GLbyte *v){
	    glad_debug_glVertexAttrib4Nbv(index, v);
	}
#endif // glVertexAttrib4Nbv

#ifdef glVertexAttrib4NbvARB
#undef glVertexAttrib4NbvARB
	static inline void glVertexAttrib4NbvARB(GLuint index, const GLbyte *v){
	    glad_debug_glVertexAttrib4NbvARB(index, v);
	}
#endif // glVertexAttrib4NbvARB

#ifdef glVertexAttrib4Niv
#undef glVertexAttrib4Niv
	static inline void glVertexAttrib4Niv(GLuint index, const GLint *v){
	    glad_debug_glVertexAttrib4Niv(index, v);
	}
#endif // glVertexAttrib4Niv

#ifdef glVertexAttrib4NivARB
#undef glVertexAttrib4NivARB
	static inline void glVertexAttrib4NivARB(GLuint index, const GLint *v){
	    glad_debug_glVertexAttrib4NivARB(index, v);
	}
#endif // glVertexAttrib4NivARB

#ifdef glVertexAttrib4Nsv
#undef glVertexAttrib4Nsv
	static inline void glVertexAttrib4Nsv(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib4Nsv(index, v);
	}
#endif // glVertexAttrib4Nsv

#ifdef glVertexAttrib4NsvARB
#undef glVertexAttrib4NsvARB
	static inline void glVertexAttrib4NsvARB(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib4NsvARB(index, v);
	}
#endif // glVertexAttrib4NsvARB

#ifdef glVertexAttrib4Nub
#undef glVertexAttrib4Nub
	static inline void glVertexAttrib4Nub(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w){
	    glad_debug_glVertexAttrib4Nub(index, x, y, z, w);
	}
#endif // glVertexAttrib4Nub

#ifdef glVertexAttrib4NubARB
#undef glVertexAttrib4NubARB
	static inline void glVertexAttrib4NubARB(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w){
	    glad_debug_glVertexAttrib4NubARB(index, x, y, z, w);
	}
#endif // glVertexAttrib4NubARB

#ifdef glVertexAttrib4Nubv
#undef glVertexAttrib4Nubv
	static inline void glVertexAttrib4Nubv(GLuint index, const GLubyte *v){
	    glad_debug_glVertexAttrib4Nubv(index, v);
	}
#endif // glVertexAttrib4Nubv

#ifdef glVertexAttrib4NubvARB
#undef glVertexAttrib4NubvARB
	static inline void glVertexAttrib4NubvARB(GLuint index, const GLubyte *v){
	    glad_debug_glVertexAttrib4NubvARB(index, v);
	}
#endif // glVertexAttrib4NubvARB

#ifdef glVertexAttrib4Nuiv
#undef glVertexAttrib4Nuiv
	static inline void glVertexAttrib4Nuiv(GLuint index, const GLuint *v){
	    glad_debug_glVertexAttrib4Nuiv(index, v);
	}
#endif // glVertexAttrib4Nuiv

#ifdef glVertexAttrib4NuivARB
#undef glVertexAttrib4NuivARB
	static inline void glVertexAttrib4NuivARB(GLuint index, const GLuint *v){
	    glad_debug_glVertexAttrib4NuivARB(index, v);
	}
#endif // glVertexAttrib4NuivARB

#ifdef glVertexAttrib4Nusv
#undef glVertexAttrib4Nusv
	static inline void glVertexAttrib4Nusv(GLuint index, const GLushort *v){
	    glad_debug_glVertexAttrib4Nusv(index, v);
	}
#endif // glVertexAttrib4Nusv

#ifdef glVertexAttrib4NusvARB
#undef glVertexAttrib4NusvARB
	static inline void glVertexAttrib4NusvARB(GLuint index, const GLushort *v){
	    glad_debug_glVertexAttrib4NusvARB(index, v);
	}
#endif // glVertexAttrib4NusvARB

#ifdef glVertexAttrib4bv
#undef glVertexAttrib4bv
	static inline void glVertexAttrib4bv(GLuint index, const GLbyte *v){
	    glad_debug_glVertexAttrib4bv(index, v);
	}
#endif // glVertexAttrib4bv

#ifdef glVertexAttrib4bvARB
#undef glVertexAttrib4bvARB
	static inline void glVertexAttrib4bvARB(GLuint index, const GLbyte *v){
	    glad_debug_glVertexAttrib4bvARB(index, v);
	}
#endif // glVertexAttrib4bvARB

#ifdef glVertexAttrib4d
#undef glVertexAttrib4d
	static inline void glVertexAttrib4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glVertexAttrib4d(index, x, y, z, w);
	}
#endif // glVertexAttrib4d

#ifdef glVertexAttrib4dARB
#undef glVertexAttrib4dARB
	static inline void glVertexAttrib4dARB(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glVertexAttrib4dARB(index, x, y, z, w);
	}
#endif // glVertexAttrib4dARB

#ifdef glVertexAttrib4dNV
#undef glVertexAttrib4dNV
	static inline void glVertexAttrib4dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glVertexAttrib4dNV(index, x, y, z, w);
	}
#endif // glVertexAttrib4dNV

#ifdef glVertexAttrib4dv
#undef glVertexAttrib4dv
	static inline void glVertexAttrib4dv(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttrib4dv(index, v);
	}
#endif // glVertexAttrib4dv

#ifdef glVertexAttrib4dvARB
#undef glVertexAttrib4dvARB
	static inline void glVertexAttrib4dvARB(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttrib4dvARB(index, v);
	}
#endif // glVertexAttrib4dvARB

#ifdef glVertexAttrib4dvNV
#undef glVertexAttrib4dvNV
	static inline void glVertexAttrib4dvNV(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttrib4dvNV(index, v);
	}
#endif // glVertexAttrib4dvNV

#ifdef glVertexAttrib4f
#undef glVertexAttrib4f
	static inline void glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glVertexAttrib4f(index, x, y, z, w);
	}
#endif // glVertexAttrib4f

#ifdef glVertexAttrib4fARB
#undef glVertexAttrib4fARB
	static inline void glVertexAttrib4fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glVertexAttrib4fARB(index, x, y, z, w);
	}
#endif // glVertexAttrib4fARB

#ifdef glVertexAttrib4fNV
#undef glVertexAttrib4fNV
	static inline void glVertexAttrib4fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glVertexAttrib4fNV(index, x, y, z, w);
	}
#endif // glVertexAttrib4fNV

#ifdef glVertexAttrib4fv
#undef glVertexAttrib4fv
	static inline void glVertexAttrib4fv(GLuint index, const GLfloat *v){
	    glad_debug_glVertexAttrib4fv(index, v);
	}
#endif // glVertexAttrib4fv

#ifdef glVertexAttrib4fvARB
#undef glVertexAttrib4fvARB
	static inline void glVertexAttrib4fvARB(GLuint index, const GLfloat *v){
	    glad_debug_glVertexAttrib4fvARB(index, v);
	}
#endif // glVertexAttrib4fvARB

#ifdef glVertexAttrib4fvNV
#undef glVertexAttrib4fvNV
	static inline void glVertexAttrib4fvNV(GLuint index, const GLfloat *v){
	    glad_debug_glVertexAttrib4fvNV(index, v);
	}
#endif // glVertexAttrib4fvNV

#ifdef glVertexAttrib4hNV
#undef glVertexAttrib4hNV
	static inline void glVertexAttrib4hNV(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w){
	    glad_debug_glVertexAttrib4hNV(index, x, y, z, w);
	}
#endif // glVertexAttrib4hNV

#ifdef glVertexAttrib4hvNV
#undef glVertexAttrib4hvNV
	static inline void glVertexAttrib4hvNV(GLuint index, const GLhalfNV *v){
	    glad_debug_glVertexAttrib4hvNV(index, v);
	}
#endif // glVertexAttrib4hvNV

#ifdef glVertexAttrib4iv
#undef glVertexAttrib4iv
	static inline void glVertexAttrib4iv(GLuint index, const GLint *v){
	    glad_debug_glVertexAttrib4iv(index, v);
	}
#endif // glVertexAttrib4iv

#ifdef glVertexAttrib4ivARB
#undef glVertexAttrib4ivARB
	static inline void glVertexAttrib4ivARB(GLuint index, const GLint *v){
	    glad_debug_glVertexAttrib4ivARB(index, v);
	}
#endif // glVertexAttrib4ivARB

#ifdef glVertexAttrib4s
#undef glVertexAttrib4s
	static inline void glVertexAttrib4s(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w){
	    glad_debug_glVertexAttrib4s(index, x, y, z, w);
	}
#endif // glVertexAttrib4s

#ifdef glVertexAttrib4sARB
#undef glVertexAttrib4sARB
	static inline void glVertexAttrib4sARB(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w){
	    glad_debug_glVertexAttrib4sARB(index, x, y, z, w);
	}
#endif // glVertexAttrib4sARB

#ifdef glVertexAttrib4sNV
#undef glVertexAttrib4sNV
	static inline void glVertexAttrib4sNV(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w){
	    glad_debug_glVertexAttrib4sNV(index, x, y, z, w);
	}
#endif // glVertexAttrib4sNV

#ifdef glVertexAttrib4sv
#undef glVertexAttrib4sv
	static inline void glVertexAttrib4sv(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib4sv(index, v);
	}
#endif // glVertexAttrib4sv

#ifdef glVertexAttrib4svARB
#undef glVertexAttrib4svARB
	static inline void glVertexAttrib4svARB(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib4svARB(index, v);
	}
#endif // glVertexAttrib4svARB

#ifdef glVertexAttrib4svNV
#undef glVertexAttrib4svNV
	static inline void glVertexAttrib4svNV(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttrib4svNV(index, v);
	}
#endif // glVertexAttrib4svNV

#ifdef glVertexAttrib4ubNV
#undef glVertexAttrib4ubNV
	static inline void glVertexAttrib4ubNV(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w){
	    glad_debug_glVertexAttrib4ubNV(index, x, y, z, w);
	}
#endif // glVertexAttrib4ubNV

#ifdef glVertexAttrib4ubv
#undef glVertexAttrib4ubv
	static inline void glVertexAttrib4ubv(GLuint index, const GLubyte *v){
	    glad_debug_glVertexAttrib4ubv(index, v);
	}
#endif // glVertexAttrib4ubv

#ifdef glVertexAttrib4ubvARB
#undef glVertexAttrib4ubvARB
	static inline void glVertexAttrib4ubvARB(GLuint index, const GLubyte *v){
	    glad_debug_glVertexAttrib4ubvARB(index, v);
	}
#endif // glVertexAttrib4ubvARB

#ifdef glVertexAttrib4ubvNV
#undef glVertexAttrib4ubvNV
	static inline void glVertexAttrib4ubvNV(GLuint index, const GLubyte *v){
	    glad_debug_glVertexAttrib4ubvNV(index, v);
	}
#endif // glVertexAttrib4ubvNV

#ifdef glVertexAttrib4uiv
#undef glVertexAttrib4uiv
	static inline void glVertexAttrib4uiv(GLuint index, const GLuint *v){
	    glad_debug_glVertexAttrib4uiv(index, v);
	}
#endif // glVertexAttrib4uiv

#ifdef glVertexAttrib4uivARB
#undef glVertexAttrib4uivARB
	static inline void glVertexAttrib4uivARB(GLuint index, const GLuint *v){
	    glad_debug_glVertexAttrib4uivARB(index, v);
	}
#endif // glVertexAttrib4uivARB

#ifdef glVertexAttrib4usv
#undef glVertexAttrib4usv
	static inline void glVertexAttrib4usv(GLuint index, const GLushort *v){
	    glad_debug_glVertexAttrib4usv(index, v);
	}
#endif // glVertexAttrib4usv

#ifdef glVertexAttrib4usvARB
#undef glVertexAttrib4usvARB
	static inline void glVertexAttrib4usvARB(GLuint index, const GLushort *v){
	    glad_debug_glVertexAttrib4usvARB(index, v);
	}
#endif // glVertexAttrib4usvARB

#ifdef glVertexAttribArrayObjectATI
#undef glVertexAttribArrayObjectATI
	static inline void glVertexAttribArrayObjectATI(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset){
	    glad_debug_glVertexAttribArrayObjectATI(index, size, type, normalized, stride, buffer, offset);
	}
#endif // glVertexAttribArrayObjectATI

#ifdef glVertexAttribBinding
#undef glVertexAttribBinding
	static inline void glVertexAttribBinding(GLuint attribindex, GLuint bindingindex){
	    glad_debug_glVertexAttribBinding(attribindex, bindingindex);
	}
#endif // glVertexAttribBinding

#ifdef glVertexAttribDivisor
#undef glVertexAttribDivisor
	static inline void glVertexAttribDivisor(GLuint index, GLuint divisor){
	    glad_debug_glVertexAttribDivisor(index, divisor);
	}
#endif // glVertexAttribDivisor

#ifdef glVertexAttribDivisorANGLE
#undef glVertexAttribDivisorANGLE
	static inline void glVertexAttribDivisorANGLE(GLuint index, GLuint divisor){
	    glad_debug_glVertexAttribDivisorANGLE(index, divisor);
	}
#endif // glVertexAttribDivisorANGLE

#ifdef glVertexAttribDivisorARB
#undef glVertexAttribDivisorARB
	static inline void glVertexAttribDivisorARB(GLuint index, GLuint divisor){
	    glad_debug_glVertexAttribDivisorARB(index, divisor);
	}
#endif // glVertexAttribDivisorARB

#ifdef glVertexAttribDivisorEXT
#undef glVertexAttribDivisorEXT
	static inline void glVertexAttribDivisorEXT(GLuint index, GLuint divisor){
	    glad_debug_glVertexAttribDivisorEXT(index, divisor);
	}
#endif // glVertexAttribDivisorEXT

#ifdef glVertexAttribDivisorNV
#undef glVertexAttribDivisorNV
	static inline void glVertexAttribDivisorNV(GLuint index, GLuint divisor){
	    glad_debug_glVertexAttribDivisorNV(index, divisor);
	}
#endif // glVertexAttribDivisorNV

#ifdef glVertexAttribFormat
#undef glVertexAttribFormat
	static inline void glVertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset){
	    glad_debug_glVertexAttribFormat(attribindex, size, type, normalized, relativeoffset);
	}
#endif // glVertexAttribFormat

#ifdef glVertexAttribFormatNV
#undef glVertexAttribFormatNV
	static inline void glVertexAttribFormatNV(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride){
	    glad_debug_glVertexAttribFormatNV(index, size, type, normalized, stride);
	}
#endif // glVertexAttribFormatNV

#ifdef glVertexAttribI1i
#undef glVertexAttribI1i
	static inline void glVertexAttribI1i(GLuint index, GLint x){
	    glad_debug_glVertexAttribI1i(index, x);
	}
#endif // glVertexAttribI1i

#ifdef glVertexAttribI1iEXT
#undef glVertexAttribI1iEXT
	static inline void glVertexAttribI1iEXT(GLuint index, GLint x){
	    glad_debug_glVertexAttribI1iEXT(index, x);
	}
#endif // glVertexAttribI1iEXT

#ifdef glVertexAttribI1iv
#undef glVertexAttribI1iv
	static inline void glVertexAttribI1iv(GLuint index, const GLint *v){
	    glad_debug_glVertexAttribI1iv(index, v);
	}
#endif // glVertexAttribI1iv

#ifdef glVertexAttribI1ivEXT
#undef glVertexAttribI1ivEXT
	static inline void glVertexAttribI1ivEXT(GLuint index, const GLint *v){
	    glad_debug_glVertexAttribI1ivEXT(index, v);
	}
#endif // glVertexAttribI1ivEXT

#ifdef glVertexAttribI1ui
#undef glVertexAttribI1ui
	static inline void glVertexAttribI1ui(GLuint index, GLuint x){
	    glad_debug_glVertexAttribI1ui(index, x);
	}
#endif // glVertexAttribI1ui

#ifdef glVertexAttribI1uiEXT
#undef glVertexAttribI1uiEXT
	static inline void glVertexAttribI1uiEXT(GLuint index, GLuint x){
	    glad_debug_glVertexAttribI1uiEXT(index, x);
	}
#endif // glVertexAttribI1uiEXT

#ifdef glVertexAttribI1uiv
#undef glVertexAttribI1uiv
	static inline void glVertexAttribI1uiv(GLuint index, const GLuint *v){
	    glad_debug_glVertexAttribI1uiv(index, v);
	}
#endif // glVertexAttribI1uiv

#ifdef glVertexAttribI1uivEXT
#undef glVertexAttribI1uivEXT
	static inline void glVertexAttribI1uivEXT(GLuint index, const GLuint *v){
	    glad_debug_glVertexAttribI1uivEXT(index, v);
	}
#endif // glVertexAttribI1uivEXT

#ifdef glVertexAttribI2i
#undef glVertexAttribI2i
	static inline void glVertexAttribI2i(GLuint index, GLint x, GLint y){
	    glad_debug_glVertexAttribI2i(index, x, y);
	}
#endif // glVertexAttribI2i

#ifdef glVertexAttribI2iEXT
#undef glVertexAttribI2iEXT
	static inline void glVertexAttribI2iEXT(GLuint index, GLint x, GLint y){
	    glad_debug_glVertexAttribI2iEXT(index, x, y);
	}
#endif // glVertexAttribI2iEXT

#ifdef glVertexAttribI2iv
#undef glVertexAttribI2iv
	static inline void glVertexAttribI2iv(GLuint index, const GLint *v){
	    glad_debug_glVertexAttribI2iv(index, v);
	}
#endif // glVertexAttribI2iv

#ifdef glVertexAttribI2ivEXT
#undef glVertexAttribI2ivEXT
	static inline void glVertexAttribI2ivEXT(GLuint index, const GLint *v){
	    glad_debug_glVertexAttribI2ivEXT(index, v);
	}
#endif // glVertexAttribI2ivEXT

#ifdef glVertexAttribI2ui
#undef glVertexAttribI2ui
	static inline void glVertexAttribI2ui(GLuint index, GLuint x, GLuint y){
	    glad_debug_glVertexAttribI2ui(index, x, y);
	}
#endif // glVertexAttribI2ui

#ifdef glVertexAttribI2uiEXT
#undef glVertexAttribI2uiEXT
	static inline void glVertexAttribI2uiEXT(GLuint index, GLuint x, GLuint y){
	    glad_debug_glVertexAttribI2uiEXT(index, x, y);
	}
#endif // glVertexAttribI2uiEXT

#ifdef glVertexAttribI2uiv
#undef glVertexAttribI2uiv
	static inline void glVertexAttribI2uiv(GLuint index, const GLuint *v){
	    glad_debug_glVertexAttribI2uiv(index, v);
	}
#endif // glVertexAttribI2uiv

#ifdef glVertexAttribI2uivEXT
#undef glVertexAttribI2uivEXT
	static inline void glVertexAttribI2uivEXT(GLuint index, const GLuint *v){
	    glad_debug_glVertexAttribI2uivEXT(index, v);
	}
#endif // glVertexAttribI2uivEXT

#ifdef glVertexAttribI3i
#undef glVertexAttribI3i
	static inline void glVertexAttribI3i(GLuint index, GLint x, GLint y, GLint z){
	    glad_debug_glVertexAttribI3i(index, x, y, z);
	}
#endif // glVertexAttribI3i

#ifdef glVertexAttribI3iEXT
#undef glVertexAttribI3iEXT
	static inline void glVertexAttribI3iEXT(GLuint index, GLint x, GLint y, GLint z){
	    glad_debug_glVertexAttribI3iEXT(index, x, y, z);
	}
#endif // glVertexAttribI3iEXT

#ifdef glVertexAttribI3iv
#undef glVertexAttribI3iv
	static inline void glVertexAttribI3iv(GLuint index, const GLint *v){
	    glad_debug_glVertexAttribI3iv(index, v);
	}
#endif // glVertexAttribI3iv

#ifdef glVertexAttribI3ivEXT
#undef glVertexAttribI3ivEXT
	static inline void glVertexAttribI3ivEXT(GLuint index, const GLint *v){
	    glad_debug_glVertexAttribI3ivEXT(index, v);
	}
#endif // glVertexAttribI3ivEXT

#ifdef glVertexAttribI3ui
#undef glVertexAttribI3ui
	static inline void glVertexAttribI3ui(GLuint index, GLuint x, GLuint y, GLuint z){
	    glad_debug_glVertexAttribI3ui(index, x, y, z);
	}
#endif // glVertexAttribI3ui

#ifdef glVertexAttribI3uiEXT
#undef glVertexAttribI3uiEXT
	static inline void glVertexAttribI3uiEXT(GLuint index, GLuint x, GLuint y, GLuint z){
	    glad_debug_glVertexAttribI3uiEXT(index, x, y, z);
	}
#endif // glVertexAttribI3uiEXT

#ifdef glVertexAttribI3uiv
#undef glVertexAttribI3uiv
	static inline void glVertexAttribI3uiv(GLuint index, const GLuint *v){
	    glad_debug_glVertexAttribI3uiv(index, v);
	}
#endif // glVertexAttribI3uiv

#ifdef glVertexAttribI3uivEXT
#undef glVertexAttribI3uivEXT
	static inline void glVertexAttribI3uivEXT(GLuint index, const GLuint *v){
	    glad_debug_glVertexAttribI3uivEXT(index, v);
	}
#endif // glVertexAttribI3uivEXT

#ifdef glVertexAttribI4bv
#undef glVertexAttribI4bv
	static inline void glVertexAttribI4bv(GLuint index, const GLbyte *v){
	    glad_debug_glVertexAttribI4bv(index, v);
	}
#endif // glVertexAttribI4bv

#ifdef glVertexAttribI4bvEXT
#undef glVertexAttribI4bvEXT
	static inline void glVertexAttribI4bvEXT(GLuint index, const GLbyte *v){
	    glad_debug_glVertexAttribI4bvEXT(index, v);
	}
#endif // glVertexAttribI4bvEXT

#ifdef glVertexAttribI4i
#undef glVertexAttribI4i
	static inline void glVertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w){
	    glad_debug_glVertexAttribI4i(index, x, y, z, w);
	}
#endif // glVertexAttribI4i

#ifdef glVertexAttribI4iEXT
#undef glVertexAttribI4iEXT
	static inline void glVertexAttribI4iEXT(GLuint index, GLint x, GLint y, GLint z, GLint w){
	    glad_debug_glVertexAttribI4iEXT(index, x, y, z, w);
	}
#endif // glVertexAttribI4iEXT

#ifdef glVertexAttribI4iv
#undef glVertexAttribI4iv
	static inline void glVertexAttribI4iv(GLuint index, const GLint *v){
	    glad_debug_glVertexAttribI4iv(index, v);
	}
#endif // glVertexAttribI4iv

#ifdef glVertexAttribI4ivEXT
#undef glVertexAttribI4ivEXT
	static inline void glVertexAttribI4ivEXT(GLuint index, const GLint *v){
	    glad_debug_glVertexAttribI4ivEXT(index, v);
	}
#endif // glVertexAttribI4ivEXT

#ifdef glVertexAttribI4sv
#undef glVertexAttribI4sv
	static inline void glVertexAttribI4sv(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttribI4sv(index, v);
	}
#endif // glVertexAttribI4sv

#ifdef glVertexAttribI4svEXT
#undef glVertexAttribI4svEXT
	static inline void glVertexAttribI4svEXT(GLuint index, const GLshort *v){
	    glad_debug_glVertexAttribI4svEXT(index, v);
	}
#endif // glVertexAttribI4svEXT

#ifdef glVertexAttribI4ubv
#undef glVertexAttribI4ubv
	static inline void glVertexAttribI4ubv(GLuint index, const GLubyte *v){
	    glad_debug_glVertexAttribI4ubv(index, v);
	}
#endif // glVertexAttribI4ubv

#ifdef glVertexAttribI4ubvEXT
#undef glVertexAttribI4ubvEXT
	static inline void glVertexAttribI4ubvEXT(GLuint index, const GLubyte *v){
	    glad_debug_glVertexAttribI4ubvEXT(index, v);
	}
#endif // glVertexAttribI4ubvEXT

#ifdef glVertexAttribI4ui
#undef glVertexAttribI4ui
	static inline void glVertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w){
	    glad_debug_glVertexAttribI4ui(index, x, y, z, w);
	}
#endif // glVertexAttribI4ui

#ifdef glVertexAttribI4uiEXT
#undef glVertexAttribI4uiEXT
	static inline void glVertexAttribI4uiEXT(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w){
	    glad_debug_glVertexAttribI4uiEXT(index, x, y, z, w);
	}
#endif // glVertexAttribI4uiEXT

#ifdef glVertexAttribI4uiv
#undef glVertexAttribI4uiv
	static inline void glVertexAttribI4uiv(GLuint index, const GLuint *v){
	    glad_debug_glVertexAttribI4uiv(index, v);
	}
#endif // glVertexAttribI4uiv

#ifdef glVertexAttribI4uivEXT
#undef glVertexAttribI4uivEXT
	static inline void glVertexAttribI4uivEXT(GLuint index, const GLuint *v){
	    glad_debug_glVertexAttribI4uivEXT(index, v);
	}
#endif // glVertexAttribI4uivEXT

#ifdef glVertexAttribI4usv
#undef glVertexAttribI4usv
	static inline void glVertexAttribI4usv(GLuint index, const GLushort *v){
	    glad_debug_glVertexAttribI4usv(index, v);
	}
#endif // glVertexAttribI4usv

#ifdef glVertexAttribI4usvEXT
#undef glVertexAttribI4usvEXT
	static inline void glVertexAttribI4usvEXT(GLuint index, const GLushort *v){
	    glad_debug_glVertexAttribI4usvEXT(index, v);
	}
#endif // glVertexAttribI4usvEXT

#ifdef glVertexAttribIFormat
#undef glVertexAttribIFormat
	static inline void glVertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset){
	    glad_debug_glVertexAttribIFormat(attribindex, size, type, relativeoffset);
	}
#endif // glVertexAttribIFormat

#ifdef glVertexAttribIFormatNV
#undef glVertexAttribIFormatNV
	static inline void glVertexAttribIFormatNV(GLuint index, GLint size, GLenum type, GLsizei stride){
	    glad_debug_glVertexAttribIFormatNV(index, size, type, stride);
	}
#endif // glVertexAttribIFormatNV

#ifdef glVertexAttribL1d
#undef glVertexAttribL1d
	static inline void glVertexAttribL1d(GLuint index, GLdouble x){
	    glad_debug_glVertexAttribL1d(index, x);
	}
#endif // glVertexAttribL1d

#ifdef glVertexAttribL1dEXT
#undef glVertexAttribL1dEXT
	static inline void glVertexAttribL1dEXT(GLuint index, GLdouble x){
	    glad_debug_glVertexAttribL1dEXT(index, x);
	}
#endif // glVertexAttribL1dEXT

#ifdef glVertexAttribL1dv
#undef glVertexAttribL1dv
	static inline void glVertexAttribL1dv(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttribL1dv(index, v);
	}
#endif // glVertexAttribL1dv

#ifdef glVertexAttribL1dvEXT
#undef glVertexAttribL1dvEXT
	static inline void glVertexAttribL1dvEXT(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttribL1dvEXT(index, v);
	}
#endif // glVertexAttribL1dvEXT

#ifdef glVertexAttribL1i64NV
#undef glVertexAttribL1i64NV
	static inline void glVertexAttribL1i64NV(GLuint index, GLint64EXT x){
	    glad_debug_glVertexAttribL1i64NV(index, x);
	}
#endif // glVertexAttribL1i64NV

#ifdef glVertexAttribL1i64vNV
#undef glVertexAttribL1i64vNV
	static inline void glVertexAttribL1i64vNV(GLuint index, const GLint64EXT *v){
	    glad_debug_glVertexAttribL1i64vNV(index, v);
	}
#endif // glVertexAttribL1i64vNV

#ifdef glVertexAttribL1ui64ARB
#undef glVertexAttribL1ui64ARB
	static inline void glVertexAttribL1ui64ARB(GLuint index, GLuint64EXT x){
	    glad_debug_glVertexAttribL1ui64ARB(index, x);
	}
#endif // glVertexAttribL1ui64ARB

#ifdef glVertexAttribL1ui64NV
#undef glVertexAttribL1ui64NV
	static inline void glVertexAttribL1ui64NV(GLuint index, GLuint64EXT x){
	    glad_debug_glVertexAttribL1ui64NV(index, x);
	}
#endif // glVertexAttribL1ui64NV

#ifdef glVertexAttribL1ui64vARB
#undef glVertexAttribL1ui64vARB
	static inline void glVertexAttribL1ui64vARB(GLuint index, const GLuint64EXT *v){
	    glad_debug_glVertexAttribL1ui64vARB(index, v);
	}
#endif // glVertexAttribL1ui64vARB

#ifdef glVertexAttribL1ui64vNV
#undef glVertexAttribL1ui64vNV
	static inline void glVertexAttribL1ui64vNV(GLuint index, const GLuint64EXT *v){
	    glad_debug_glVertexAttribL1ui64vNV(index, v);
	}
#endif // glVertexAttribL1ui64vNV

#ifdef glVertexAttribL2d
#undef glVertexAttribL2d
	static inline void glVertexAttribL2d(GLuint index, GLdouble x, GLdouble y){
	    glad_debug_glVertexAttribL2d(index, x, y);
	}
#endif // glVertexAttribL2d

#ifdef glVertexAttribL2dEXT
#undef glVertexAttribL2dEXT
	static inline void glVertexAttribL2dEXT(GLuint index, GLdouble x, GLdouble y){
	    glad_debug_glVertexAttribL2dEXT(index, x, y);
	}
#endif // glVertexAttribL2dEXT

#ifdef glVertexAttribL2dv
#undef glVertexAttribL2dv
	static inline void glVertexAttribL2dv(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttribL2dv(index, v);
	}
#endif // glVertexAttribL2dv

#ifdef glVertexAttribL2dvEXT
#undef glVertexAttribL2dvEXT
	static inline void glVertexAttribL2dvEXT(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttribL2dvEXT(index, v);
	}
#endif // glVertexAttribL2dvEXT

#ifdef glVertexAttribL2i64NV
#undef glVertexAttribL2i64NV
	static inline void glVertexAttribL2i64NV(GLuint index, GLint64EXT x, GLint64EXT y){
	    glad_debug_glVertexAttribL2i64NV(index, x, y);
	}
#endif // glVertexAttribL2i64NV

#ifdef glVertexAttribL2i64vNV
#undef glVertexAttribL2i64vNV
	static inline void glVertexAttribL2i64vNV(GLuint index, const GLint64EXT *v){
	    glad_debug_glVertexAttribL2i64vNV(index, v);
	}
#endif // glVertexAttribL2i64vNV

#ifdef glVertexAttribL2ui64NV
#undef glVertexAttribL2ui64NV
	static inline void glVertexAttribL2ui64NV(GLuint index, GLuint64EXT x, GLuint64EXT y){
	    glad_debug_glVertexAttribL2ui64NV(index, x, y);
	}
#endif // glVertexAttribL2ui64NV

#ifdef glVertexAttribL2ui64vNV
#undef glVertexAttribL2ui64vNV
	static inline void glVertexAttribL2ui64vNV(GLuint index, const GLuint64EXT *v){
	    glad_debug_glVertexAttribL2ui64vNV(index, v);
	}
#endif // glVertexAttribL2ui64vNV

#ifdef glVertexAttribL3d
#undef glVertexAttribL3d
	static inline void glVertexAttribL3d(GLuint index, GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glVertexAttribL3d(index, x, y, z);
	}
#endif // glVertexAttribL3d

#ifdef glVertexAttribL3dEXT
#undef glVertexAttribL3dEXT
	static inline void glVertexAttribL3dEXT(GLuint index, GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glVertexAttribL3dEXT(index, x, y, z);
	}
#endif // glVertexAttribL3dEXT

#ifdef glVertexAttribL3dv
#undef glVertexAttribL3dv
	static inline void glVertexAttribL3dv(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttribL3dv(index, v);
	}
#endif // glVertexAttribL3dv

#ifdef glVertexAttribL3dvEXT
#undef glVertexAttribL3dvEXT
	static inline void glVertexAttribL3dvEXT(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttribL3dvEXT(index, v);
	}
#endif // glVertexAttribL3dvEXT

#ifdef glVertexAttribL3i64NV
#undef glVertexAttribL3i64NV
	static inline void glVertexAttribL3i64NV(GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z){
	    glad_debug_glVertexAttribL3i64NV(index, x, y, z);
	}
#endif // glVertexAttribL3i64NV

#ifdef glVertexAttribL3i64vNV
#undef glVertexAttribL3i64vNV
	static inline void glVertexAttribL3i64vNV(GLuint index, const GLint64EXT *v){
	    glad_debug_glVertexAttribL3i64vNV(index, v);
	}
#endif // glVertexAttribL3i64vNV

#ifdef glVertexAttribL3ui64NV
#undef glVertexAttribL3ui64NV
	static inline void glVertexAttribL3ui64NV(GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z){
	    glad_debug_glVertexAttribL3ui64NV(index, x, y, z);
	}
#endif // glVertexAttribL3ui64NV

#ifdef glVertexAttribL3ui64vNV
#undef glVertexAttribL3ui64vNV
	static inline void glVertexAttribL3ui64vNV(GLuint index, const GLuint64EXT *v){
	    glad_debug_glVertexAttribL3ui64vNV(index, v);
	}
#endif // glVertexAttribL3ui64vNV

#ifdef glVertexAttribL4d
#undef glVertexAttribL4d
	static inline void glVertexAttribL4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glVertexAttribL4d(index, x, y, z, w);
	}
#endif // glVertexAttribL4d

#ifdef glVertexAttribL4dEXT
#undef glVertexAttribL4dEXT
	static inline void glVertexAttribL4dEXT(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glVertexAttribL4dEXT(index, x, y, z, w);
	}
#endif // glVertexAttribL4dEXT

#ifdef glVertexAttribL4dv
#undef glVertexAttribL4dv
	static inline void glVertexAttribL4dv(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttribL4dv(index, v);
	}
#endif // glVertexAttribL4dv

#ifdef glVertexAttribL4dvEXT
#undef glVertexAttribL4dvEXT
	static inline void glVertexAttribL4dvEXT(GLuint index, const GLdouble *v){
	    glad_debug_glVertexAttribL4dvEXT(index, v);
	}
#endif // glVertexAttribL4dvEXT

#ifdef glVertexAttribL4i64NV
#undef glVertexAttribL4i64NV
	static inline void glVertexAttribL4i64NV(GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w){
	    glad_debug_glVertexAttribL4i64NV(index, x, y, z, w);
	}
#endif // glVertexAttribL4i64NV

#ifdef glVertexAttribL4i64vNV
#undef glVertexAttribL4i64vNV
	static inline void glVertexAttribL4i64vNV(GLuint index, const GLint64EXT *v){
	    glad_debug_glVertexAttribL4i64vNV(index, v);
	}
#endif // glVertexAttribL4i64vNV

#ifdef glVertexAttribL4ui64NV
#undef glVertexAttribL4ui64NV
	static inline void glVertexAttribL4ui64NV(GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w){
	    glad_debug_glVertexAttribL4ui64NV(index, x, y, z, w);
	}
#endif // glVertexAttribL4ui64NV

#ifdef glVertexAttribL4ui64vNV
#undef glVertexAttribL4ui64vNV
	static inline void glVertexAttribL4ui64vNV(GLuint index, const GLuint64EXT *v){
	    glad_debug_glVertexAttribL4ui64vNV(index, v);
	}
#endif // glVertexAttribL4ui64vNV

#ifdef glVertexAttribLFormat
#undef glVertexAttribLFormat
	static inline void glVertexAttribLFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset){
	    glad_debug_glVertexAttribLFormat(attribindex, size, type, relativeoffset);
	}
#endif // glVertexAttribLFormat

#ifdef glVertexAttribLFormatNV
#undef glVertexAttribLFormatNV
	static inline void glVertexAttribLFormatNV(GLuint index, GLint size, GLenum type, GLsizei stride){
	    glad_debug_glVertexAttribLFormatNV(index, size, type, stride);
	}
#endif // glVertexAttribLFormatNV

#ifdef glVertexAttribP1ui
#undef glVertexAttribP1ui
	static inline void glVertexAttribP1ui(GLuint index, GLenum type, GLboolean normalized, GLuint value){
	    glad_debug_glVertexAttribP1ui(index, type, normalized, value);
	}
#endif // glVertexAttribP1ui

#ifdef glVertexAttribP1uiv
#undef glVertexAttribP1uiv
	static inline void glVertexAttribP1uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value){
	    glad_debug_glVertexAttribP1uiv(index, type, normalized, value);
	}
#endif // glVertexAttribP1uiv

#ifdef glVertexAttribP2ui
#undef glVertexAttribP2ui
	static inline void glVertexAttribP2ui(GLuint index, GLenum type, GLboolean normalized, GLuint value){
	    glad_debug_glVertexAttribP2ui(index, type, normalized, value);
	}
#endif // glVertexAttribP2ui

#ifdef glVertexAttribP2uiv
#undef glVertexAttribP2uiv
	static inline void glVertexAttribP2uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value){
	    glad_debug_glVertexAttribP2uiv(index, type, normalized, value);
	}
#endif // glVertexAttribP2uiv

#ifdef glVertexAttribP3ui
#undef glVertexAttribP3ui
	static inline void glVertexAttribP3ui(GLuint index, GLenum type, GLboolean normalized, GLuint value){
	    glad_debug_glVertexAttribP3ui(index, type, normalized, value);
	}
#endif // glVertexAttribP3ui

#ifdef glVertexAttribP3uiv
#undef glVertexAttribP3uiv
	static inline void glVertexAttribP3uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value){
	    glad_debug_glVertexAttribP3uiv(index, type, normalized, value);
	}
#endif // glVertexAttribP3uiv

#ifdef glVertexAttribP4ui
#undef glVertexAttribP4ui
	static inline void glVertexAttribP4ui(GLuint index, GLenum type, GLboolean normalized, GLuint value){
	    glad_debug_glVertexAttribP4ui(index, type, normalized, value);
	}
#endif // glVertexAttribP4ui

#ifdef glVertexAttribP4uiv
#undef glVertexAttribP4uiv
	static inline void glVertexAttribP4uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value){
	    glad_debug_glVertexAttribP4uiv(index, type, normalized, value);
	}
#endif // glVertexAttribP4uiv

#ifdef glVertexAttribParameteriAMD
#undef glVertexAttribParameteriAMD
	static inline void glVertexAttribParameteriAMD(GLuint index, GLenum pname, GLint param){
	    glad_debug_glVertexAttribParameteriAMD(index, pname, param);
	}
#endif // glVertexAttribParameteriAMD

#ifdef glVertexAttribs1dvNV
#undef glVertexAttribs1dvNV
	static inline void glVertexAttribs1dvNV(GLuint index, GLsizei count, const GLdouble *v){
	    glad_debug_glVertexAttribs1dvNV(index, count, v);
	}
#endif // glVertexAttribs1dvNV

#ifdef glVertexAttribs1fvNV
#undef glVertexAttribs1fvNV
	static inline void glVertexAttribs1fvNV(GLuint index, GLsizei count, const GLfloat *v){
	    glad_debug_glVertexAttribs1fvNV(index, count, v);
	}
#endif // glVertexAttribs1fvNV

#ifdef glVertexAttribs1hvNV
#undef glVertexAttribs1hvNV
	static inline void glVertexAttribs1hvNV(GLuint index, GLsizei n, const GLhalfNV *v){
	    glad_debug_glVertexAttribs1hvNV(index, n, v);
	}
#endif // glVertexAttribs1hvNV

#ifdef glVertexAttribs1svNV
#undef glVertexAttribs1svNV
	static inline void glVertexAttribs1svNV(GLuint index, GLsizei count, const GLshort *v){
	    glad_debug_glVertexAttribs1svNV(index, count, v);
	}
#endif // glVertexAttribs1svNV

#ifdef glVertexAttribs2dvNV
#undef glVertexAttribs2dvNV
	static inline void glVertexAttribs2dvNV(GLuint index, GLsizei count, const GLdouble *v){
	    glad_debug_glVertexAttribs2dvNV(index, count, v);
	}
#endif // glVertexAttribs2dvNV

#ifdef glVertexAttribs2fvNV
#undef glVertexAttribs2fvNV
	static inline void glVertexAttribs2fvNV(GLuint index, GLsizei count, const GLfloat *v){
	    glad_debug_glVertexAttribs2fvNV(index, count, v);
	}
#endif // glVertexAttribs2fvNV

#ifdef glVertexAttribs2hvNV
#undef glVertexAttribs2hvNV
	static inline void glVertexAttribs2hvNV(GLuint index, GLsizei n, const GLhalfNV *v){
	    glad_debug_glVertexAttribs2hvNV(index, n, v);
	}
#endif // glVertexAttribs2hvNV

#ifdef glVertexAttribs2svNV
#undef glVertexAttribs2svNV
	static inline void glVertexAttribs2svNV(GLuint index, GLsizei count, const GLshort *v){
	    glad_debug_glVertexAttribs2svNV(index, count, v);
	}
#endif // glVertexAttribs2svNV

#ifdef glVertexAttribs3dvNV
#undef glVertexAttribs3dvNV
	static inline void glVertexAttribs3dvNV(GLuint index, GLsizei count, const GLdouble *v){
	    glad_debug_glVertexAttribs3dvNV(index, count, v);
	}
#endif // glVertexAttribs3dvNV

#ifdef glVertexAttribs3fvNV
#undef glVertexAttribs3fvNV
	static inline void glVertexAttribs3fvNV(GLuint index, GLsizei count, const GLfloat *v){
	    glad_debug_glVertexAttribs3fvNV(index, count, v);
	}
#endif // glVertexAttribs3fvNV

#ifdef glVertexAttribs3hvNV
#undef glVertexAttribs3hvNV
	static inline void glVertexAttribs3hvNV(GLuint index, GLsizei n, const GLhalfNV *v){
	    glad_debug_glVertexAttribs3hvNV(index, n, v);
	}
#endif // glVertexAttribs3hvNV

#ifdef glVertexAttribs3svNV
#undef glVertexAttribs3svNV
	static inline void glVertexAttribs3svNV(GLuint index, GLsizei count, const GLshort *v){
	    glad_debug_glVertexAttribs3svNV(index, count, v);
	}
#endif // glVertexAttribs3svNV

#ifdef glVertexAttribs4dvNV
#undef glVertexAttribs4dvNV
	static inline void glVertexAttribs4dvNV(GLuint index, GLsizei count, const GLdouble *v){
	    glad_debug_glVertexAttribs4dvNV(index, count, v);
	}
#endif // glVertexAttribs4dvNV

#ifdef glVertexAttribs4fvNV
#undef glVertexAttribs4fvNV
	static inline void glVertexAttribs4fvNV(GLuint index, GLsizei count, const GLfloat *v){
	    glad_debug_glVertexAttribs4fvNV(index, count, v);
	}
#endif // glVertexAttribs4fvNV

#ifdef glVertexAttribs4hvNV
#undef glVertexAttribs4hvNV
	static inline void glVertexAttribs4hvNV(GLuint index, GLsizei n, const GLhalfNV *v){
	    glad_debug_glVertexAttribs4hvNV(index, n, v);
	}
#endif // glVertexAttribs4hvNV

#ifdef glVertexAttribs4svNV
#undef glVertexAttribs4svNV
	static inline void glVertexAttribs4svNV(GLuint index, GLsizei count, const GLshort *v){
	    glad_debug_glVertexAttribs4svNV(index, count, v);
	}
#endif // glVertexAttribs4svNV

#ifdef glVertexAttribs4ubvNV
#undef glVertexAttribs4ubvNV
	static inline void glVertexAttribs4ubvNV(GLuint index, GLsizei count, const GLubyte *v){
	    glad_debug_glVertexAttribs4ubvNV(index, count, v);
	}
#endif // glVertexAttribs4ubvNV

#ifdef glVertexBindingDivisor
#undef glVertexBindingDivisor
	static inline void glVertexBindingDivisor(GLuint bindingindex, GLuint divisor){
	    glad_debug_glVertexBindingDivisor(bindingindex, divisor);
	}
#endif // glVertexBindingDivisor

#ifdef glVertexBlendARB
#undef glVertexBlendARB
	static inline void glVertexBlendARB(GLint count){
	    glad_debug_glVertexBlendARB(count);
	}
#endif // glVertexBlendARB

#ifdef glVertexBlendEnvfATI
#undef glVertexBlendEnvfATI
	static inline void glVertexBlendEnvfATI(GLenum pname, GLfloat param){
	    glad_debug_glVertexBlendEnvfATI(pname, param);
	}
#endif // glVertexBlendEnvfATI

#ifdef glVertexBlendEnviATI
#undef glVertexBlendEnviATI
	static inline void glVertexBlendEnviATI(GLenum pname, GLint param){
	    glad_debug_glVertexBlendEnviATI(pname, param);
	}
#endif // glVertexBlendEnviATI

#ifdef glVertexFormatNV
#undef glVertexFormatNV
	static inline void glVertexFormatNV(GLint size, GLenum type, GLsizei stride){
	    glad_debug_glVertexFormatNV(size, type, stride);
	}
#endif // glVertexFormatNV

#ifdef glVertexP2ui
#undef glVertexP2ui
	static inline void glVertexP2ui(GLenum type, GLuint value){
	    glad_debug_glVertexP2ui(type, value);
	}
#endif // glVertexP2ui

#ifdef glVertexP2uiv
#undef glVertexP2uiv
	static inline void glVertexP2uiv(GLenum type, const GLuint *value){
	    glad_debug_glVertexP2uiv(type, value);
	}
#endif // glVertexP2uiv

#ifdef glVertexP3ui
#undef glVertexP3ui
	static inline void glVertexP3ui(GLenum type, GLuint value){
	    glad_debug_glVertexP3ui(type, value);
	}
#endif // glVertexP3ui

#ifdef glVertexP3uiv
#undef glVertexP3uiv
	static inline void glVertexP3uiv(GLenum type, const GLuint *value){
	    glad_debug_glVertexP3uiv(type, value);
	}
#endif // glVertexP3uiv

#ifdef glVertexP4ui
#undef glVertexP4ui
	static inline void glVertexP4ui(GLenum type, GLuint value){
	    glad_debug_glVertexP4ui(type, value);
	}
#endif // glVertexP4ui

#ifdef glVertexP4uiv
#undef glVertexP4uiv
	static inline void glVertexP4uiv(GLenum type, const GLuint *value){
	    glad_debug_glVertexP4uiv(type, value);
	}
#endif // glVertexP4uiv

#ifdef glVertexStream1dATI
#undef glVertexStream1dATI
	static inline void glVertexStream1dATI(GLenum stream, GLdouble x){
	    glad_debug_glVertexStream1dATI(stream, x);
	}
#endif // glVertexStream1dATI

#ifdef glVertexStream1dvATI
#undef glVertexStream1dvATI
	static inline void glVertexStream1dvATI(GLenum stream, const GLdouble *coords){
	    glad_debug_glVertexStream1dvATI(stream, coords);
	}
#endif // glVertexStream1dvATI

#ifdef glVertexStream1fATI
#undef glVertexStream1fATI
	static inline void glVertexStream1fATI(GLenum stream, GLfloat x){
	    glad_debug_glVertexStream1fATI(stream, x);
	}
#endif // glVertexStream1fATI

#ifdef glVertexStream1fvATI
#undef glVertexStream1fvATI
	static inline void glVertexStream1fvATI(GLenum stream, const GLfloat *coords){
	    glad_debug_glVertexStream1fvATI(stream, coords);
	}
#endif // glVertexStream1fvATI

#ifdef glVertexStream1iATI
#undef glVertexStream1iATI
	static inline void glVertexStream1iATI(GLenum stream, GLint x){
	    glad_debug_glVertexStream1iATI(stream, x);
	}
#endif // glVertexStream1iATI

#ifdef glVertexStream1ivATI
#undef glVertexStream1ivATI
	static inline void glVertexStream1ivATI(GLenum stream, const GLint *coords){
	    glad_debug_glVertexStream1ivATI(stream, coords);
	}
#endif // glVertexStream1ivATI

#ifdef glVertexStream1sATI
#undef glVertexStream1sATI
	static inline void glVertexStream1sATI(GLenum stream, GLshort x){
	    glad_debug_glVertexStream1sATI(stream, x);
	}
#endif // glVertexStream1sATI

#ifdef glVertexStream1svATI
#undef glVertexStream1svATI
	static inline void glVertexStream1svATI(GLenum stream, const GLshort *coords){
	    glad_debug_glVertexStream1svATI(stream, coords);
	}
#endif // glVertexStream1svATI

#ifdef glVertexStream2dATI
#undef glVertexStream2dATI
	static inline void glVertexStream2dATI(GLenum stream, GLdouble x, GLdouble y){
	    glad_debug_glVertexStream2dATI(stream, x, y);
	}
#endif // glVertexStream2dATI

#ifdef glVertexStream2dvATI
#undef glVertexStream2dvATI
	static inline void glVertexStream2dvATI(GLenum stream, const GLdouble *coords){
	    glad_debug_glVertexStream2dvATI(stream, coords);
	}
#endif // glVertexStream2dvATI

#ifdef glVertexStream2fATI
#undef glVertexStream2fATI
	static inline void glVertexStream2fATI(GLenum stream, GLfloat x, GLfloat y){
	    glad_debug_glVertexStream2fATI(stream, x, y);
	}
#endif // glVertexStream2fATI

#ifdef glVertexStream2fvATI
#undef glVertexStream2fvATI
	static inline void glVertexStream2fvATI(GLenum stream, const GLfloat *coords){
	    glad_debug_glVertexStream2fvATI(stream, coords);
	}
#endif // glVertexStream2fvATI

#ifdef glVertexStream2iATI
#undef glVertexStream2iATI
	static inline void glVertexStream2iATI(GLenum stream, GLint x, GLint y){
	    glad_debug_glVertexStream2iATI(stream, x, y);
	}
#endif // glVertexStream2iATI

#ifdef glVertexStream2ivATI
#undef glVertexStream2ivATI
	static inline void glVertexStream2ivATI(GLenum stream, const GLint *coords){
	    glad_debug_glVertexStream2ivATI(stream, coords);
	}
#endif // glVertexStream2ivATI

#ifdef glVertexStream2sATI
#undef glVertexStream2sATI
	static inline void glVertexStream2sATI(GLenum stream, GLshort x, GLshort y){
	    glad_debug_glVertexStream2sATI(stream, x, y);
	}
#endif // glVertexStream2sATI

#ifdef glVertexStream2svATI
#undef glVertexStream2svATI
	static inline void glVertexStream2svATI(GLenum stream, const GLshort *coords){
	    glad_debug_glVertexStream2svATI(stream, coords);
	}
#endif // glVertexStream2svATI

#ifdef glVertexStream3dATI
#undef glVertexStream3dATI
	static inline void glVertexStream3dATI(GLenum stream, GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glVertexStream3dATI(stream, x, y, z);
	}
#endif // glVertexStream3dATI

#ifdef glVertexStream3dvATI
#undef glVertexStream3dvATI
	static inline void glVertexStream3dvATI(GLenum stream, const GLdouble *coords){
	    glad_debug_glVertexStream3dvATI(stream, coords);
	}
#endif // glVertexStream3dvATI

#ifdef glVertexStream3fATI
#undef glVertexStream3fATI
	static inline void glVertexStream3fATI(GLenum stream, GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glVertexStream3fATI(stream, x, y, z);
	}
#endif // glVertexStream3fATI

#ifdef glVertexStream3fvATI
#undef glVertexStream3fvATI
	static inline void glVertexStream3fvATI(GLenum stream, const GLfloat *coords){
	    glad_debug_glVertexStream3fvATI(stream, coords);
	}
#endif // glVertexStream3fvATI

#ifdef glVertexStream3iATI
#undef glVertexStream3iATI
	static inline void glVertexStream3iATI(GLenum stream, GLint x, GLint y, GLint z){
	    glad_debug_glVertexStream3iATI(stream, x, y, z);
	}
#endif // glVertexStream3iATI

#ifdef glVertexStream3ivATI
#undef glVertexStream3ivATI
	static inline void glVertexStream3ivATI(GLenum stream, const GLint *coords){
	    glad_debug_glVertexStream3ivATI(stream, coords);
	}
#endif // glVertexStream3ivATI

#ifdef glVertexStream3sATI
#undef glVertexStream3sATI
	static inline void glVertexStream3sATI(GLenum stream, GLshort x, GLshort y, GLshort z){
	    glad_debug_glVertexStream3sATI(stream, x, y, z);
	}
#endif // glVertexStream3sATI

#ifdef glVertexStream3svATI
#undef glVertexStream3svATI
	static inline void glVertexStream3svATI(GLenum stream, const GLshort *coords){
	    glad_debug_glVertexStream3svATI(stream, coords);
	}
#endif // glVertexStream3svATI

#ifdef glVertexStream4dATI
#undef glVertexStream4dATI
	static inline void glVertexStream4dATI(GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glVertexStream4dATI(stream, x, y, z, w);
	}
#endif // glVertexStream4dATI

#ifdef glVertexStream4dvATI
#undef glVertexStream4dvATI
	static inline void glVertexStream4dvATI(GLenum stream, const GLdouble *coords){
	    glad_debug_glVertexStream4dvATI(stream, coords);
	}
#endif // glVertexStream4dvATI

#ifdef glVertexStream4fATI
#undef glVertexStream4fATI
	static inline void glVertexStream4fATI(GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glVertexStream4fATI(stream, x, y, z, w);
	}
#endif // glVertexStream4fATI

#ifdef glVertexStream4fvATI
#undef glVertexStream4fvATI
	static inline void glVertexStream4fvATI(GLenum stream, const GLfloat *coords){
	    glad_debug_glVertexStream4fvATI(stream, coords);
	}
#endif // glVertexStream4fvATI

#ifdef glVertexStream4iATI
#undef glVertexStream4iATI
	static inline void glVertexStream4iATI(GLenum stream, GLint x, GLint y, GLint z, GLint w){
	    glad_debug_glVertexStream4iATI(stream, x, y, z, w);
	}
#endif // glVertexStream4iATI

#ifdef glVertexStream4ivATI
#undef glVertexStream4ivATI
	static inline void glVertexStream4ivATI(GLenum stream, const GLint *coords){
	    glad_debug_glVertexStream4ivATI(stream, coords);
	}
#endif // glVertexStream4ivATI

#ifdef glVertexStream4sATI
#undef glVertexStream4sATI
	static inline void glVertexStream4sATI(GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w){
	    glad_debug_glVertexStream4sATI(stream, x, y, z, w);
	}
#endif // glVertexStream4sATI

#ifdef glVertexStream4svATI
#undef glVertexStream4svATI
	static inline void glVertexStream4svATI(GLenum stream, const GLshort *coords){
	    glad_debug_glVertexStream4svATI(stream, coords);
	}
#endif // glVertexStream4svATI

#ifdef glVertexWeightfEXT
#undef glVertexWeightfEXT
	static inline void glVertexWeightfEXT(GLfloat weight){
	    glad_debug_glVertexWeightfEXT(weight);
	}
#endif // glVertexWeightfEXT

#ifdef glVertexWeightfvEXT
#undef glVertexWeightfvEXT
	static inline void glVertexWeightfvEXT(const GLfloat *weight){
	    glad_debug_glVertexWeightfvEXT(weight);
	}
#endif // glVertexWeightfvEXT

#ifdef glVertexWeighthNV
#undef glVertexWeighthNV
	static inline void glVertexWeighthNV(GLhalfNV weight){
	    glad_debug_glVertexWeighthNV(weight);
	}
#endif // glVertexWeighthNV

#ifdef glVertexWeighthvNV
#undef glVertexWeighthvNV
	static inline void glVertexWeighthvNV(const GLhalfNV *weight){
	    glad_debug_glVertexWeighthvNV(weight);
	}
#endif // glVertexWeighthvNV

#ifdef glVideoCaptureStreamParameterdvNV
#undef glVideoCaptureStreamParameterdvNV
	static inline void glVideoCaptureStreamParameterdvNV(GLuint video_capture_slot, GLuint stream, GLenum pname, const GLdouble *params){
	    glad_debug_glVideoCaptureStreamParameterdvNV(video_capture_slot, stream, pname, params);
	}
#endif // glVideoCaptureStreamParameterdvNV

#ifdef glVideoCaptureStreamParameterfvNV
#undef glVideoCaptureStreamParameterfvNV
	static inline void glVideoCaptureStreamParameterfvNV(GLuint video_capture_slot, GLuint stream, GLenum pname, const GLfloat *params){
	    glad_debug_glVideoCaptureStreamParameterfvNV(video_capture_slot, stream, pname, params);
	}
#endif // glVideoCaptureStreamParameterfvNV

#ifdef glVideoCaptureStreamParameterivNV
#undef glVideoCaptureStreamParameterivNV
	static inline void glVideoCaptureStreamParameterivNV(GLuint video_capture_slot, GLuint stream, GLenum pname, const GLint *params){
	    glad_debug_glVideoCaptureStreamParameterivNV(video_capture_slot, stream, pname, params);
	}
#endif // glVideoCaptureStreamParameterivNV

#ifdef glViewport
#undef glViewport
	static inline void glViewport(GLint x, GLint y, GLsizei width, GLsizei height){
	    glad_debug_glViewport(x, y, width, height);
	}
#endif // glViewport

#ifdef glViewportArrayv
#undef glViewportArrayv
	static inline void glViewportArrayv(GLuint first, GLsizei count, const GLfloat *v){
	    glad_debug_glViewportArrayv(first, count, v);
	}
#endif // glViewportArrayv

#ifdef glViewportArrayvNV
#undef glViewportArrayvNV
	static inline void glViewportArrayvNV(GLuint first, GLsizei count, const GLfloat *v){
	    glad_debug_glViewportArrayvNV(first, count, v);
	}
#endif // glViewportArrayvNV

#ifdef glViewportArrayvOES
#undef glViewportArrayvOES
	static inline void glViewportArrayvOES(GLuint first, GLsizei count, const GLfloat *v){
	    glad_debug_glViewportArrayvOES(first, count, v);
	}
#endif // glViewportArrayvOES

#ifdef glViewportIndexedf
#undef glViewportIndexedf
	static inline void glViewportIndexedf(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h){
	    glad_debug_glViewportIndexedf(index, x, y, w, h);
	}
#endif // glViewportIndexedf

#ifdef glViewportIndexedfOES
#undef glViewportIndexedfOES
	static inline void glViewportIndexedfOES(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h){
	    glad_debug_glViewportIndexedfOES(index, x, y, w, h);
	}
#endif // glViewportIndexedfOES

#ifdef glViewportIndexedfNV
#undef glViewportIndexedfNV
	static inline void glViewportIndexedfNV(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h){
	    glad_debug_glViewportIndexedfNV(index, x, y, w, h);
	}
#endif // glViewportIndexedfNV

#ifdef glViewportIndexedfv
#undef glViewportIndexedfv
	static inline void glViewportIndexedfv(GLuint index, const GLfloat *v){
	    glad_debug_glViewportIndexedfv(index, v);
	}
#endif // glViewportIndexedfv

#ifdef glViewportIndexedfvOES
#undef glViewportIndexedfvOES
	static inline void glViewportIndexedfvOES(GLuint index, const GLfloat *v){
	    glad_debug_glViewportIndexedfvOES(index, v);
	}
#endif // glViewportIndexedfvOES

#ifdef glViewportIndexedfvNV
#undef glViewportIndexedfvNV
	static inline void glViewportIndexedfvNV(GLuint index, const GLfloat *v){
	    glad_debug_glViewportIndexedfvNV(index, v);
	}
#endif // glViewportIndexedfvNV

#ifdef glViewportPositionWScaleNV
#undef glViewportPositionWScaleNV
	static inline void glViewportPositionWScaleNV(GLuint index, GLfloat xcoeff, GLfloat ycoeff){
	    glad_debug_glViewportPositionWScaleNV(index, xcoeff, ycoeff);
	}
#endif // glViewportPositionWScaleNV

#ifdef glViewportSwizzleNV
#undef glViewportSwizzleNV
	static inline void glViewportSwizzleNV(GLuint index, GLenum swizzlex, GLenum swizzley, GLenum swizzlez, GLenum swizzlew){
	    glad_debug_glViewportSwizzleNV(index, swizzlex, swizzley, swizzlez, swizzlew);
	}
#endif // glViewportSwizzleNV

#ifdef glWaitSemaphoreEXT
#undef glWaitSemaphoreEXT
	static inline void glWaitSemaphoreEXT(GLuint semaphore, GLuint numBufferBarriers, const GLuint *buffers, GLuint numTextureBarriers, const GLuint *textures, const GLenum *srcLayouts){
	    glad_debug_glWaitSemaphoreEXT(semaphore, numBufferBarriers, buffers, numTextureBarriers, textures, srcLayouts);
	}
#endif // glWaitSemaphoreEXT

#ifdef glWaitSemaphoreui64NVX
#undef glWaitSemaphoreui64NVX
	static inline void glWaitSemaphoreui64NVX(GLuint waitGpu, GLsizei fenceObjectCount, const GLuint *semaphoreArray, const GLuint64 *fenceValueArray){
	    glad_debug_glWaitSemaphoreui64NVX(waitGpu, fenceObjectCount, semaphoreArray, fenceValueArray);
	}
#endif // glWaitSemaphoreui64NVX

#ifdef glWaitSync
#undef glWaitSync
	static inline void glWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout){
	    glad_debug_glWaitSync(sync, flags, timeout);
	}
#endif // glWaitSync

#ifdef glWaitSyncAPPLE
#undef glWaitSyncAPPLE
	static inline void glWaitSyncAPPLE(GLsync sync, GLbitfield flags, GLuint64 timeout){
	    glad_debug_glWaitSyncAPPLE(sync, flags, timeout);
	}
#endif // glWaitSyncAPPLE

#ifdef glWeightPathsNV
#undef glWeightPathsNV
	static inline void glWeightPathsNV(GLuint resultPath, GLsizei numPaths, const GLuint *paths, const GLfloat *weights){
	    glad_debug_glWeightPathsNV(resultPath, numPaths, paths, weights);
	}
#endif // glWeightPathsNV

#ifdef glWeightbvARB
#undef glWeightbvARB
	static inline void glWeightbvARB(GLint size, const GLbyte *weights){
	    glad_debug_glWeightbvARB(size, weights);
	}
#endif // glWeightbvARB

#ifdef glWeightdvARB
#undef glWeightdvARB
	static inline void glWeightdvARB(GLint size, const GLdouble *weights){
	    glad_debug_glWeightdvARB(size, weights);
	}
#endif // glWeightdvARB

#ifdef glWeightfvARB
#undef glWeightfvARB
	static inline void glWeightfvARB(GLint size, const GLfloat *weights){
	    glad_debug_glWeightfvARB(size, weights);
	}
#endif // glWeightfvARB

#ifdef glWeightivARB
#undef glWeightivARB
	static inline void glWeightivARB(GLint size, const GLint *weights){
	    glad_debug_glWeightivARB(size, weights);
	}
#endif // glWeightivARB

#ifdef glWeightsvARB
#undef glWeightsvARB
	static inline void glWeightsvARB(GLint size, const GLshort *weights){
	    glad_debug_glWeightsvARB(size, weights);
	}
#endif // glWeightsvARB

#ifdef glWeightubvARB
#undef glWeightubvARB
	static inline void glWeightubvARB(GLint size, const GLubyte *weights){
	    glad_debug_glWeightubvARB(size, weights);
	}
#endif // glWeightubvARB

#ifdef glWeightuivARB
#undef glWeightuivARB
	static inline void glWeightuivARB(GLint size, const GLuint *weights){
	    glad_debug_glWeightuivARB(size, weights);
	}
#endif // glWeightuivARB

#ifdef glWeightusvARB
#undef glWeightusvARB
	static inline void glWeightusvARB(GLint size, const GLushort *weights){
	    glad_debug_glWeightusvARB(size, weights);
	}
#endif // glWeightusvARB

#ifdef glWindowPos2d
#undef glWindowPos2d
	static inline void glWindowPos2d(GLdouble x, GLdouble y){
	    glad_debug_glWindowPos2d(x, y);
	}
#endif // glWindowPos2d

#ifdef glWindowPos2dARB
#undef glWindowPos2dARB
	static inline void glWindowPos2dARB(GLdouble x, GLdouble y){
	    glad_debug_glWindowPos2dARB(x, y);
	}
#endif // glWindowPos2dARB

#ifdef glWindowPos2dMESA
#undef glWindowPos2dMESA
	static inline void glWindowPos2dMESA(GLdouble x, GLdouble y){
	    glad_debug_glWindowPos2dMESA(x, y);
	}
#endif // glWindowPos2dMESA

#ifdef glWindowPos2dv
#undef glWindowPos2dv
	static inline void glWindowPos2dv(const GLdouble *v){
	    glad_debug_glWindowPos2dv(v);
	}
#endif // glWindowPos2dv

#ifdef glWindowPos2dvARB
#undef glWindowPos2dvARB
	static inline void glWindowPos2dvARB(const GLdouble *v){
	    glad_debug_glWindowPos2dvARB(v);
	}
#endif // glWindowPos2dvARB

#ifdef glWindowPos2dvMESA
#undef glWindowPos2dvMESA
	static inline void glWindowPos2dvMESA(const GLdouble *v){
	    glad_debug_glWindowPos2dvMESA(v);
	}
#endif // glWindowPos2dvMESA

#ifdef glWindowPos2f
#undef glWindowPos2f
	static inline void glWindowPos2f(GLfloat x, GLfloat y){
	    glad_debug_glWindowPos2f(x, y);
	}
#endif // glWindowPos2f

#ifdef glWindowPos2fARB
#undef glWindowPos2fARB
	static inline void glWindowPos2fARB(GLfloat x, GLfloat y){
	    glad_debug_glWindowPos2fARB(x, y);
	}
#endif // glWindowPos2fARB

#ifdef glWindowPos2fMESA
#undef glWindowPos2fMESA
	static inline void glWindowPos2fMESA(GLfloat x, GLfloat y){
	    glad_debug_glWindowPos2fMESA(x, y);
	}
#endif // glWindowPos2fMESA

#ifdef glWindowPos2fv
#undef glWindowPos2fv
	static inline void glWindowPos2fv(const GLfloat *v){
	    glad_debug_glWindowPos2fv(v);
	}
#endif // glWindowPos2fv

#ifdef glWindowPos2fvARB
#undef glWindowPos2fvARB
	static inline void glWindowPos2fvARB(const GLfloat *v){
	    glad_debug_glWindowPos2fvARB(v);
	}
#endif // glWindowPos2fvARB

#ifdef glWindowPos2fvMESA
#undef glWindowPos2fvMESA
	static inline void glWindowPos2fvMESA(const GLfloat *v){
	    glad_debug_glWindowPos2fvMESA(v);
	}
#endif // glWindowPos2fvMESA

#ifdef glWindowPos2i
#undef glWindowPos2i
	static inline void glWindowPos2i(GLint x, GLint y){
	    glad_debug_glWindowPos2i(x, y);
	}
#endif // glWindowPos2i

#ifdef glWindowPos2iARB
#undef glWindowPos2iARB
	static inline void glWindowPos2iARB(GLint x, GLint y){
	    glad_debug_glWindowPos2iARB(x, y);
	}
#endif // glWindowPos2iARB

#ifdef glWindowPos2iMESA
#undef glWindowPos2iMESA
	static inline void glWindowPos2iMESA(GLint x, GLint y){
	    glad_debug_glWindowPos2iMESA(x, y);
	}
#endif // glWindowPos2iMESA

#ifdef glWindowPos2iv
#undef glWindowPos2iv
	static inline void glWindowPos2iv(const GLint *v){
	    glad_debug_glWindowPos2iv(v);
	}
#endif // glWindowPos2iv

#ifdef glWindowPos2ivARB
#undef glWindowPos2ivARB
	static inline void glWindowPos2ivARB(const GLint *v){
	    glad_debug_glWindowPos2ivARB(v);
	}
#endif // glWindowPos2ivARB

#ifdef glWindowPos2ivMESA
#undef glWindowPos2ivMESA
	static inline void glWindowPos2ivMESA(const GLint *v){
	    glad_debug_glWindowPos2ivMESA(v);
	}
#endif // glWindowPos2ivMESA

#ifdef glWindowPos2s
#undef glWindowPos2s
	static inline void glWindowPos2s(GLshort x, GLshort y){
	    glad_debug_glWindowPos2s(x, y);
	}
#endif // glWindowPos2s

#ifdef glWindowPos2sARB
#undef glWindowPos2sARB
	static inline void glWindowPos2sARB(GLshort x, GLshort y){
	    glad_debug_glWindowPos2sARB(x, y);
	}
#endif // glWindowPos2sARB

#ifdef glWindowPos2sMESA
#undef glWindowPos2sMESA
	static inline void glWindowPos2sMESA(GLshort x, GLshort y){
	    glad_debug_glWindowPos2sMESA(x, y);
	}
#endif // glWindowPos2sMESA

#ifdef glWindowPos2sv
#undef glWindowPos2sv
	static inline void glWindowPos2sv(const GLshort *v){
	    glad_debug_glWindowPos2sv(v);
	}
#endif // glWindowPos2sv

#ifdef glWindowPos2svARB
#undef glWindowPos2svARB
	static inline void glWindowPos2svARB(const GLshort *v){
	    glad_debug_glWindowPos2svARB(v);
	}
#endif // glWindowPos2svARB

#ifdef glWindowPos2svMESA
#undef glWindowPos2svMESA
	static inline void glWindowPos2svMESA(const GLshort *v){
	    glad_debug_glWindowPos2svMESA(v);
	}
#endif // glWindowPos2svMESA

#ifdef glWindowPos3d
#undef glWindowPos3d
	static inline void glWindowPos3d(GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glWindowPos3d(x, y, z);
	}
#endif // glWindowPos3d

#ifdef glWindowPos3dARB
#undef glWindowPos3dARB
	static inline void glWindowPos3dARB(GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glWindowPos3dARB(x, y, z);
	}
#endif // glWindowPos3dARB

#ifdef glWindowPos3dMESA
#undef glWindowPos3dMESA
	static inline void glWindowPos3dMESA(GLdouble x, GLdouble y, GLdouble z){
	    glad_debug_glWindowPos3dMESA(x, y, z);
	}
#endif // glWindowPos3dMESA

#ifdef glWindowPos3dv
#undef glWindowPos3dv
	static inline void glWindowPos3dv(const GLdouble *v){
	    glad_debug_glWindowPos3dv(v);
	}
#endif // glWindowPos3dv

#ifdef glWindowPos3dvARB
#undef glWindowPos3dvARB
	static inline void glWindowPos3dvARB(const GLdouble *v){
	    glad_debug_glWindowPos3dvARB(v);
	}
#endif // glWindowPos3dvARB

#ifdef glWindowPos3dvMESA
#undef glWindowPos3dvMESA
	static inline void glWindowPos3dvMESA(const GLdouble *v){
	    glad_debug_glWindowPos3dvMESA(v);
	}
#endif // glWindowPos3dvMESA

#ifdef glWindowPos3f
#undef glWindowPos3f
	static inline void glWindowPos3f(GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glWindowPos3f(x, y, z);
	}
#endif // glWindowPos3f

#ifdef glWindowPos3fARB
#undef glWindowPos3fARB
	static inline void glWindowPos3fARB(GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glWindowPos3fARB(x, y, z);
	}
#endif // glWindowPos3fARB

#ifdef glWindowPos3fMESA
#undef glWindowPos3fMESA
	static inline void glWindowPos3fMESA(GLfloat x, GLfloat y, GLfloat z){
	    glad_debug_glWindowPos3fMESA(x, y, z);
	}
#endif // glWindowPos3fMESA

#ifdef glWindowPos3fv
#undef glWindowPos3fv
	static inline void glWindowPos3fv(const GLfloat *v){
	    glad_debug_glWindowPos3fv(v);
	}
#endif // glWindowPos3fv

#ifdef glWindowPos3fvARB
#undef glWindowPos3fvARB
	static inline void glWindowPos3fvARB(const GLfloat *v){
	    glad_debug_glWindowPos3fvARB(v);
	}
#endif // glWindowPos3fvARB

#ifdef glWindowPos3fvMESA
#undef glWindowPos3fvMESA
	static inline void glWindowPos3fvMESA(const GLfloat *v){
	    glad_debug_glWindowPos3fvMESA(v);
	}
#endif // glWindowPos3fvMESA

#ifdef glWindowPos3i
#undef glWindowPos3i
	static inline void glWindowPos3i(GLint x, GLint y, GLint z){
	    glad_debug_glWindowPos3i(x, y, z);
	}
#endif // glWindowPos3i

#ifdef glWindowPos3iARB
#undef glWindowPos3iARB
	static inline void glWindowPos3iARB(GLint x, GLint y, GLint z){
	    glad_debug_glWindowPos3iARB(x, y, z);
	}
#endif // glWindowPos3iARB

#ifdef glWindowPos3iMESA
#undef glWindowPos3iMESA
	static inline void glWindowPos3iMESA(GLint x, GLint y, GLint z){
	    glad_debug_glWindowPos3iMESA(x, y, z);
	}
#endif // glWindowPos3iMESA

#ifdef glWindowPos3iv
#undef glWindowPos3iv
	static inline void glWindowPos3iv(const GLint *v){
	    glad_debug_glWindowPos3iv(v);
	}
#endif // glWindowPos3iv

#ifdef glWindowPos3ivARB
#undef glWindowPos3ivARB
	static inline void glWindowPos3ivARB(const GLint *v){
	    glad_debug_glWindowPos3ivARB(v);
	}
#endif // glWindowPos3ivARB

#ifdef glWindowPos3ivMESA
#undef glWindowPos3ivMESA
	static inline void glWindowPos3ivMESA(const GLint *v){
	    glad_debug_glWindowPos3ivMESA(v);
	}
#endif // glWindowPos3ivMESA

#ifdef glWindowPos3s
#undef glWindowPos3s
	static inline void glWindowPos3s(GLshort x, GLshort y, GLshort z){
	    glad_debug_glWindowPos3s(x, y, z);
	}
#endif // glWindowPos3s

#ifdef glWindowPos3sARB
#undef glWindowPos3sARB
	static inline void glWindowPos3sARB(GLshort x, GLshort y, GLshort z){
	    glad_debug_glWindowPos3sARB(x, y, z);
	}
#endif // glWindowPos3sARB

#ifdef glWindowPos3sMESA
#undef glWindowPos3sMESA
	static inline void glWindowPos3sMESA(GLshort x, GLshort y, GLshort z){
	    glad_debug_glWindowPos3sMESA(x, y, z);
	}
#endif // glWindowPos3sMESA

#ifdef glWindowPos3sv
#undef glWindowPos3sv
	static inline void glWindowPos3sv(const GLshort *v){
	    glad_debug_glWindowPos3sv(v);
	}
#endif // glWindowPos3sv

#ifdef glWindowPos3svARB
#undef glWindowPos3svARB
	static inline void glWindowPos3svARB(const GLshort *v){
	    glad_debug_glWindowPos3svARB(v);
	}
#endif // glWindowPos3svARB

#ifdef glWindowPos3svMESA
#undef glWindowPos3svMESA
	static inline void glWindowPos3svMESA(const GLshort *v){
	    glad_debug_glWindowPos3svMESA(v);
	}
#endif // glWindowPos3svMESA

#ifdef glWindowPos4dMESA
#undef glWindowPos4dMESA
	static inline void glWindowPos4dMESA(GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	    glad_debug_glWindowPos4dMESA(x, y, z, w);
	}
#endif // glWindowPos4dMESA

#ifdef glWindowPos4dvMESA
#undef glWindowPos4dvMESA
	static inline void glWindowPos4dvMESA(const GLdouble *v){
	    glad_debug_glWindowPos4dvMESA(v);
	}
#endif // glWindowPos4dvMESA

#ifdef glWindowPos4fMESA
#undef glWindowPos4fMESA
	static inline void glWindowPos4fMESA(GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	    glad_debug_glWindowPos4fMESA(x, y, z, w);
	}
#endif // glWindowPos4fMESA

#ifdef glWindowPos4fvMESA
#undef glWindowPos4fvMESA
	static inline void glWindowPos4fvMESA(const GLfloat *v){
	    glad_debug_glWindowPos4fvMESA(v);
	}
#endif // glWindowPos4fvMESA

#ifdef glWindowPos4iMESA
#undef glWindowPos4iMESA
	static inline void glWindowPos4iMESA(GLint x, GLint y, GLint z, GLint w){
	    glad_debug_glWindowPos4iMESA(x, y, z, w);
	}
#endif // glWindowPos4iMESA

#ifdef glWindowPos4ivMESA
#undef glWindowPos4ivMESA
	static inline void glWindowPos4ivMESA(const GLint *v){
	    glad_debug_glWindowPos4ivMESA(v);
	}
#endif // glWindowPos4ivMESA

#ifdef glWindowPos4sMESA
#undef glWindowPos4sMESA
	static inline void glWindowPos4sMESA(GLshort x, GLshort y, GLshort z, GLshort w){
	    glad_debug_glWindowPos4sMESA(x, y, z, w);
	}
#endif // glWindowPos4sMESA

#ifdef glWindowPos4svMESA
#undef glWindowPos4svMESA
	static inline void glWindowPos4svMESA(const GLshort *v){
	    glad_debug_glWindowPos4svMESA(v);
	}
#endif // glWindowPos4svMESA

#ifdef glWindowRectanglesEXT
#undef glWindowRectanglesEXT
	static inline void glWindowRectanglesEXT(GLenum mode, GLsizei count, const GLint *box){
	    glad_debug_glWindowRectanglesEXT(mode, count, box);
	}
#endif // glWindowRectanglesEXT

#ifdef glWriteMaskEXT
#undef glWriteMaskEXT
	static inline void glWriteMaskEXT(GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW){
	    glad_debug_glWriteMaskEXT(res, in, outX, outY, outZ, outW);
	}
#endif // glWriteMaskEXT

#ifdef glDrawVkImageNV
#undef glDrawVkImageNV
	static inline void glDrawVkImageNV(GLuint64 vkImage, GLuint sampler, GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1, GLfloat z, GLfloat s0, GLfloat t0, GLfloat s1, GLfloat t1){
	    glad_debug_glDrawVkImageNV(vkImage, sampler, x0, y0, x1, y1, z, s0, t0, s1, t1);
	}
#endif // glDrawVkImageNV

#ifdef glWaitVkSemaphoreNV
#undef glWaitVkSemaphoreNV
	static inline void glWaitVkSemaphoreNV(GLuint64 vkSemaphore){
	    glad_debug_glWaitVkSemaphoreNV(vkSemaphore);
	}
#endif // glWaitVkSemaphoreNV

#ifdef glSignalVkSemaphoreNV
#undef glSignalVkSemaphoreNV
	static inline void glSignalVkSemaphoreNV(GLuint64 vkSemaphore){
	    glad_debug_glSignalVkSemaphoreNV(vkSemaphore);
	}
#endif // glSignalVkSemaphoreNV

#ifdef glSignalVkFenceNV
#undef glSignalVkFenceNV
	static inline void glSignalVkFenceNV(GLuint64 vkFence){
	    glad_debug_glSignalVkFenceNV(vkFence);
	}
#endif // glSignalVkFenceNV

#ifdef glFramebufferParameteriMESA
#undef glFramebufferParameteriMESA
	static inline void glFramebufferParameteriMESA(GLenum target, GLenum pname, GLint param){
	    glad_debug_glFramebufferParameteriMESA(target, pname, param);
	}
#endif // glFramebufferParameteriMESA

#ifdef glGetFramebufferParameterivMESA
#undef glGetFramebufferParameterivMESA
	static inline void glGetFramebufferParameterivMESA(GLenum target, GLenum pname, GLint *params){
	    glad_debug_glGetFramebufferParameterivMESA(target, pname, params);
	}
#endif // glGetFramebufferParameterivMESA

