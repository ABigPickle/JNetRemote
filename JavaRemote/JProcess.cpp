#include <src/BlackBone/Process/Process.h>
#include <src/BlackBone/Process/RPC/RemoteFunction.hpp>
#include <jni.h>
#include "JProcess.h"
#include "JavaDeclarations.h"
#include <iostream>

namespace JavaRemoteNative {
	template<typename T>
	auto JNI::GetStandardMethodFromExport(const char* name) {
		auto exportData = _InnerProcess.modules().GetExport(_JVMModule, name);
		blackbone::RemoteFunction<T> remoteFunction{ _InnerProcess, exportData->procAddress };
		return remoteFunction;
	}

	JNI::JNI(DWORD processID) {
		_InnerProcess.Attach(processID);
		_JVMModule = _InnerProcess.modules().GetModule(L"jvm.dll");
		_InnerProcess.remote().CreateRPCEnvironment(blackbone::Worker_CreateNew);
		_WorkerThread = _InnerProcess.remote().getWorker();

		jsize vmCount;
		if (this->JNI_GetCreatedJavaVMs(&_VM, 1, &vmCount) != JNI_OK || vmCount == 0) {
			throw gcnew System::Exception("Failed to find JavaVM");
		}
		vmFuncs = this->GetInvokeInterface();
		jint res = this->GetEnv((void**)&_ENV, JNI_VERSION_1_8);
		if (res == JNI_EDETACHED) {
			res = this->AttachCurrentThread((void**)&_ENV, nullptr);
		}
		if (res != JNI_OK) {
			throw gcnew System::Exception("Failed to link runtimes.");
		}
		envFuncs = this->GetNativeInterface();
	}

	/* Standard Export Functions */

	jint JNI::JNI_GetCreatedJavaVMs(JavaVM** vmBuf, jsize bufLen, jsize* nVMs) {
		auto exportedFunction = GetStandardMethodFromExport<JavaDeclarations::java_JNI_GetCreatedJavaVMs>("JNI_GetCreatedJavaVMs");
		decltype(exportedFunction)::CallArguments args{
			vmBuf,
			bufLen,
			nVMs
		};
		return exportedFunction.Call(args, _WorkerThread).result();
	}

	/* End Standard Export */
	
	/* JavaVM Specific Functions */

	jint JNI::GetEnv(void** penv, jint version) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetEnv> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(vmFuncs.GetEnv) };
		decltype(remoteFunction)::CallArguments args{
			_VM,
			penv,
			version
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jint JNI::AttachCurrentThread(void** penv, void* args) {
		blackbone::RemoteFunction<JavaDeclarations::java_AttachCurrentThread> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(vmFuncs.AttachCurrentThread) };
		decltype(remoteFunction)::CallArguments fargs{
			_VM,
			penv,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	/* End JavaVM Specific */

	/* JNIEnv Specific Functions */

	jint JNI::GetVersion() {
		blackbone::RemoteFunction<JavaDeclarations::java_GetVersion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetVersion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jclass JNI::FindClass(const char* name) {
		blackbone::RemoteFunction<JavaDeclarations::java_FindClass> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.FindClass) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			name
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jmethodID JNI::FromReflectedMethod(jobject method) {
		blackbone::RemoteFunction<JavaDeclarations::java_FromReflectedMethod> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.FromReflectedMethod) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)method
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jfieldID JNI::FromReflectedField(jobject field) {
		blackbone::RemoteFunction<JavaDeclarations::java_FromReflectedField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.FromReflectedField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)field
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jobject JNI::ToReflectedMethod(jclass cls, jmethodID methodID, jboolean isStatic) {
		blackbone::RemoteFunction<JavaDeclarations::java_ToReflectedMethod> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ToReflectedMethod) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)cls,
			(void*)methodID,
			(void*)isStatic
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jclass JNI::GetSuperclass(jclass sub) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetSuperClass> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetSuperclass) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)sub
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jboolean JNI::IsAssignableFrom(jclass sub, jclass sup) {
		blackbone::RemoteFunction<JavaDeclarations::java_IsAssignableFrom> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.IsAssignableFrom) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)sub,
			(void*)sup
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jobject JNI::ToReflectedField(jclass cls, jfieldID fieldID, jboolean isStatic) {
		blackbone::RemoteFunction<JavaDeclarations::java_ToReflectedField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ToReflectedField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)fieldID,
			(void*)isStatic
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jint JNI::Throw(jthrowable obj) {
		blackbone::RemoteFunction<JavaDeclarations::java_Throw> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.Throw) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jint JNI::ThrowNew(jclass clazz, const char *msg) {
		blackbone::RemoteFunction<JavaDeclarations::java_ThrowNew> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ThrowNew) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			msg
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jthrowable JNI::ExceptionOccurred() {
		blackbone::RemoteFunction<JavaDeclarations::java_ExceptionOccured> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ExceptionOccurred) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	void  JNI::ExceptionDescribe() {
		blackbone::RemoteFunction<JavaDeclarations::java_ExceptionDescribe> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ExceptionDescribe) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV
		};
		remoteFunction.Call(args, _WorkerThread);
	}

	void JNI::ExceptionClear() {
		blackbone::RemoteFunction<JavaDeclarations::java_ExceptionClear> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ExceptionClear) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV
		};
		remoteFunction.Call(args, _WorkerThread);
	}

	void JNI::FatalError(const char* msg) {
		blackbone::RemoteFunction<JavaDeclarations::java_FatalError> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.FatalError) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			msg
		};
		remoteFunction.Call(args, _WorkerThread);
	}

	jint JNI::PushLocalFrame(jint capacity) {
		blackbone::RemoteFunction<JavaDeclarations::java_PushLocalFrame> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.PushLocalFrame) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			capacity
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jobject JNI::PopLocalFrame(jobject result) {
		blackbone::RemoteFunction<JavaDeclarations::java_PopLocalFrame> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.PopLocalFrame) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)result
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jobject JNI::NewGlobalRef(jobject lobj) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewGlobalRef> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewGlobalRef) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)lobj
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	void  JNI::DeleteGlobalRef(jobject gref) {
		blackbone::RemoteFunction<JavaDeclarations::java_DeleteGlobalRef> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.DeleteGlobalRef) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)gref
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::DeleteLocalRef(jobject obj) {
		blackbone::RemoteFunction<JavaDeclarations::java_DeleteLocalRef> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.DeleteLocalRef) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	jboolean JNI::IsSameObject(jobject obj1, jobject obj2) {
		blackbone::RemoteFunction<JavaDeclarations::java_IsSameObject> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.IsSameObject) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj1,
			(void*)obj2
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jobject JNI::NewLocalRef(jobject ref) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewLocalRef> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewLocalRef) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)ref
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jint JNI::EnsureLocalCapacity(jint capacity) {
		blackbone::RemoteFunction<JavaDeclarations::java_EnsureLocalCapacity> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.EnsureLocalCapacity) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			capacity
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jobject JNI::AllocObject(jclass clazz) {
		blackbone::RemoteFunction<JavaDeclarations::java_AllocObject> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.AllocObject) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jobject JNI::NewObjectA(jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewObjectA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewObjectA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)clazz,
			(void*)methodID,
			/*(const void**)*/args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jclass JNI::GetObjectClass(jobject obj) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetObjectClass> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetObjectClass) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jboolean JNI::IsInstanceOf(jobject obj, jclass clazz) {
		blackbone::RemoteFunction<JavaDeclarations::java_IsInstanceOf> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.IsInstanceOf) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)clazz
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jmethodID JNI::GetMethodID(jclass clazz, const char* name, const char* sig) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetMethodID> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetMethodID) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			name,
			sig
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jobject JNI::CallObjectMethodA(jobject obj, jmethodID methodID, const jvalue * args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallObjectMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallObjectMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jboolean JNI::CallBooleanMethodA(jobject obj, jmethodID methodID, const jvalue * args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallBooleanMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallBooleanMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jbyte JNI::CallByteMethodA(jobject obj, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallByteMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallByteMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jchar JNI::CallCharMethodA(jobject obj, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallCharMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallCharMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jshort JNI::CallShortMethodA(jobject obj, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallShortMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallShortMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jint JNI::CallIntMethodA(jobject obj, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallIntMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallIntMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jlong JNI::CallLongMethodA(jobject obj, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallLongMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallLongMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jfloat JNI::CallFloatMethodA(jobject obj, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallFloatMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallFloatMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jdouble JNI::CallDoubleMethodA(jobject obj, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallDoubleMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallDoubleMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	void  JNI::CallVoidMethodA(jobject obj, jmethodID methodID, const jvalue * args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallVoidMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallVoidMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)methodID,
			args
		};
		remoteFunction.Call(fargs, _WorkerThread);
	}

	jobject JNI::CallNonvirtualObjectMethodA(jobject obj, jclass clazz, jmethodID methodID, const jvalue * args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallNonvirtualObjectMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallNonvirtualObjectMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jboolean JNI::CallNonvirtualBooleanMethodA(jobject obj, jclass clazz, jmethodID methodID, const jvalue * args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallNonvirtualBooleanMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallNonvirtualBooleanMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jbyte JNI::CallNonvirtualByteMethodA(jobject obj, jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallNonvirtualByteMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallNonvirtualByteMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jchar JNI::CallNonvirtualCharMethodA(jobject obj, jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallNonvirtualCharMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallNonvirtualCharMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jshort JNI::CallNonvirtualShortMethodA(jobject obj, jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallNonvirtualShortMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallNonvirtualShortMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jint JNI::CallNonvirtualIntMethodA(jobject obj, jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallNonvirtualIntMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallNonvirtualIntMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jlong JNI::CallNonvirtualLongMethodA(jobject obj, jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallNonvirtualLongMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallNonvirtualLongMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jfloat JNI::CallNonvirtualFloatMethodA(jobject obj, jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallNonvirtualFloatMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallNonvirtualFloatMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jdouble JNI::CallNonvirtualDoubleMethodA(jobject obj, jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallNonvirtualDoubleMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallNonvirtualDoubleMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	void  JNI::CallNonvirtualVoidMethodA(jobject obj, jclass clazz, jmethodID methodID, const jvalue * args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallNonvirtualFloatMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallNonvirtualFloatMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)obj,
			(void*)clazz,
			(void*)methodID,
			args
		};
		remoteFunction.Call(fargs, _WorkerThread);
	}

	jfieldID JNI::GetFieldID(jclass clazz, const char *name, const char *sig) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetFieldID> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetFieldID) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			name,
			sig
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jobject JNI::GetObjectField(jobject obj, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetObjectField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetObjectField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jboolean JNI::GetBooleanField(jobject obj, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetBooleanField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetBooleanField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jbyte JNI::GetByteField(jobject obj, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetByteField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetByteField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jchar JNI::GetCharField(jobject obj, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetCharField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetCharField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jshort JNI::GetShortField(jobject obj, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetShortField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetShortField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jint JNI::GetIntField(jobject obj, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetIntField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetIntField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jlong JNI::GetLongField(jobject obj, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetLongField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetLongField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jfloat JNI::GetFloatField(jobject obj, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetFloatField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetFloatField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jdouble JNI::GetDoubleField(jobject obj, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetDoubleField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetDoubleField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	void  JNI::SetObjectField(jobject obj, jfieldID fieldID, jobject val) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetObjectField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetObjectField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID,
			(void*)val
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetBooleanField(jobject obj, jfieldID fieldID, jboolean val) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetBooleanField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetBooleanField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID,
			val
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetByteField(jobject obj, jfieldID fieldID, jbyte val) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetByteField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetByteField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID,
			val
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetCharField(jobject obj, jfieldID fieldID, jchar val) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetCharField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetCharField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID,
			val
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetShortField(jobject obj, jfieldID fieldID, jshort val) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetShortField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetShortField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID,
			val
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetIntField(jobject obj, jfieldID fieldID, jint val) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetIntField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetIntField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID,
			val
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetLongField(jobject obj, jfieldID fieldID, jlong val) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetLongField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetLongField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID,
			val
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetFloatField(jobject obj, jfieldID fieldID, jfloat val) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetFloatField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetFloatField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID,
			val
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetDoubleField(jobject obj, jfieldID fieldID, jdouble val) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetDoubleField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetDoubleField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)obj,
			(void*)fieldID,
			val
		};
		remoteFunction.Call(args, _WorkerThread);
	}

	jmethodID JNI::GetStaticMethodID(jclass clazz, const char *name, const char *sig) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStaticMethodID> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStaticMethodID) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			name,
			sig
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jobject JNI::CallStaticObjectMethodA(jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallStaticObjectMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallStaticObjectMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jboolean JNI::CallStaticBooleanMethodA(jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallStaticBooleanMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallStaticBooleanMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jbyte JNI::CallStaticByteMethodA(jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallStaticByteMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallStaticByteMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jchar JNI::CallStaticCharMethodA(jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallStaticCharMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallStaticCharMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jshort JNI::CallStaticShortMethodA(jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallStaticShortMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallStaticShortMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jint JNI::CallStaticIntMethodA(jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallStaticIntMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallStaticIntMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jlong JNI::CallStaticLongMethodA(jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallStaticLongMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallStaticLongMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jfloat JNI::CallStaticFloatMethodA(jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallStaticFloatMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallStaticFloatMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	jdouble JNI::CallStaticDoubleMethodA(jclass clazz, jmethodID methodID, const jvalue *args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallStaticDoubleMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallStaticDoubleMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)clazz,
			(void*)methodID,
			args
		};
		return remoteFunction.Call(fargs, _WorkerThread).result();
	}

	void  JNI::CallStaticVoidMethodA(jclass cls, jmethodID methodID, const jvalue * args) {
		blackbone::RemoteFunction<JavaDeclarations::java_CallStaticVoidMethodA> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.CallStaticVoidMethodA) };
		decltype(remoteFunction)::CallArguments fargs{
			(void*)_ENV,
			(void*)cls,
			(void*)methodID,
			args
		};
		remoteFunction.Call(fargs, _WorkerThread);
	}

	jfieldID JNI::GetStaticFieldID(jclass clazz, const char *name, const char *sig) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStaticFieldID> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStaticFieldID) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			name,
			sig
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jobject JNI::GetStaticObjectField(jclass clazz, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStaticObjectField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStaticObjectField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jboolean JNI::GetStaticBooleanField(jclass clazz, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStaticBooleanField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStaticBooleanField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jbyte JNI::GetStaticByteField(jclass clazz, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStaticByteField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStaticByteField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jchar JNI::GetStaticCharField(jclass clazz, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStaticCharField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStaticCharField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jshort JNI::GetStaticShortField(jclass clazz, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStaticShortField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStaticShortField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jint JNI::GetStaticIntField(jclass clazz, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStaticIntField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStaticIntField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jlong JNI::GetStaticLongField(jclass clazz, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStaticLongField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStaticLongField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jfloat JNI::GetStaticFloatField(jclass clazz, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStaticFloatField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStaticFloatField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jdouble JNI::GetStaticDoubleField(jclass clazz, jfieldID fieldID) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStaticDoubleField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStaticDoubleField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	void  JNI::SetStaticObjectField(jclass clazz, jfieldID fieldID, jobject value) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetStaticObjectField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetStaticObjectField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID,
			(void*)value
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetStaticBooleanField(jclass clazz, jfieldID fieldID, jboolean value) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetStaticBooleanField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetStaticBooleanField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID,
			value
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetStaticByteField(jclass clazz, jfieldID fieldID, jbyte value) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetStaticByteField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetStaticByteField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID,
			value
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetStaticCharField(jclass clazz, jfieldID fieldID, jchar value) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetStaticCharField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetStaticCharField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID,
			value
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetStaticShortField(jclass clazz, jfieldID fieldID, jshort value) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetStaticShortField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetStaticShortField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID,
			value
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetStaticIntField(jclass clazz, jfieldID fieldID, jint value) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetStaticIntField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetStaticIntField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID,
			value
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetStaticLongField(jclass clazz, jfieldID fieldID, jlong value) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetStaticLongField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetStaticLongField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID,
			value
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetStaticFloatField(jclass clazz, jfieldID fieldID, jfloat value) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetStaticFloatField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetStaticFloatField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID,
			value
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetStaticDoubleField(jclass clazz, jfieldID fieldID, jdouble value) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetStaticDoubleField> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetStaticDoubleField) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)clazz,
			(void*)fieldID,
			value
		};
		remoteFunction.Call(args, _WorkerThread);
	}

	jstring JNI::NewString(const jchar *unicode, jsize len) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewString> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewString) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			unicode,
			len
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jsize JNI::GetStringLength(jstring str) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStringLength> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStringLength) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)str
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	const jchar * JNI::GetStringChars(jstring str, jboolean *isCopy) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStringChars> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStringChars) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)str,
			isCopy
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	void  JNI::ReleaseStringChars(jstring str, const jchar *chars) {
		blackbone::RemoteFunction<JavaDeclarations::java_ReleaseStringChars> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ReleaseStringChars) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)str,
			chars
		};
		remoteFunction.Call(args, _WorkerThread);
	}

	jstring JNI::NewStringUTF(const char *utf) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewStringUTF> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewStringUTF) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			utf,
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jsize JNI::GetStringUTFLength(jstring str) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStringUTFLength> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStringUTFLength) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)str
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	const char*  JNI::GetStringUTFChars(jstring str, jboolean *isCopy) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStringUTFChars> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStringUTFChars) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)str,
			isCopy
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	void  JNI::ReleaseStringUTFChars(jstring str, const char* chars) {
		blackbone::RemoteFunction<JavaDeclarations::java_ReleaseStringUTFChars> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ReleaseStringUTFChars) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)str,
			chars
		};
		remoteFunction.Call(args, _WorkerThread);
	}


	jsize JNI::GetArrayLength(jarray array) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetArrayLength> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetArrayLength) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jobjectArray JNI::NewObjectArray(jsize len, jclass clazz, jobject init) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewObjectArray> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewObjectArray) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			len,
			(void*)clazz,
			(void*)init
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jobject JNI::GetObjectArrayElement(jobjectArray array, jsize index) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetObjectArrayElement> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetObjectArrayElement) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			index
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	void  JNI::SetObjectArrayElement(jobjectArray array, jsize index, jobject val) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetObjectArrayElement> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetObjectArrayElement) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			index,
			(void*)val
		};
		remoteFunction.Call(args, _WorkerThread);
	}

	jbooleanArray JNI::NewBooleanArray(jsize len) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewBooleanArray> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewBooleanArray) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			len
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jbyteArray JNI::NewByteArray(jsize len) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewByteArray> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewByteArray) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			len
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jcharArray JNI::NewCharArray(jsize len) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewCharArray> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewCharArray) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			len
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jshortArray JNI::NewShortArray(jsize len) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewShortArray> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewShortArray) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			len
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jintArray JNI::NewIntArray(jsize len) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewIntArray> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewIntArray) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			len
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jlongArray JNI::NewLongArray(jsize len) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewLongArray> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewLongArray) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			len
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jfloatArray JNI::NewFloatArray(jsize len) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewFloatArray> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewFloatArray) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			len
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jdoubleArray JNI::NewDoubleArray(jsize len) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewDoubleArray> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewDoubleArray) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			len
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jboolean *  JNI::GetBooleanArrayElements(jbooleanArray array, jboolean *isCopy) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetBooleanArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetBooleanArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			isCopy
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jbyte *  JNI::GetByteArrayElements(jbyteArray array, jboolean *isCopy) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetByteArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetByteArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			isCopy
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jchar *  JNI::GetCharArrayElements(jcharArray array, jboolean *isCopy) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetCharArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetCharArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			isCopy
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jshort *  JNI::GetShortArrayElements(jshortArray array, jboolean *isCopy) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetShortArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetShortArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			isCopy
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jint *  JNI::GetIntArrayElements(jintArray array, jboolean *isCopy) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetIntArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetIntArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			isCopy
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jlong *  JNI::GetLongArrayElements(jlongArray array, jboolean *isCopy) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetLongArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetLongArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			isCopy
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jfloat *  JNI::GetFloatArrayElements(jfloatArray array, jboolean *isCopy) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetFloatArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetFloatArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			isCopy
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jdouble *  JNI::GetDoubleArrayElements(jdoubleArray array, jboolean *isCopy) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetDoubleArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetDoubleArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			isCopy
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	void  JNI::ReleaseBooleanArrayElements(jbooleanArray array, jboolean *elems, jint mode) {
		blackbone::RemoteFunction<JavaDeclarations::java_ReleaseBooleanArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ReleaseBooleanArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			elems,
			mode
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::ReleaseByteArrayElements(jbyteArray array, jbyte *elems, jint mode) {
		blackbone::RemoteFunction<JavaDeclarations::java_ReleaseByteArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ReleaseByteArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			elems,
			mode
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::ReleaseCharArrayElements(jcharArray array, jchar *elems, jint mode) {
		blackbone::RemoteFunction<JavaDeclarations::java_ReleaseCharArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ReleaseCharArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			elems,
			mode
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::ReleaseShortArrayElements(jshortArray array, jshort *elems, jint mode) {
		blackbone::RemoteFunction<JavaDeclarations::java_ReleaseShortArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ReleaseShortArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			elems,
			mode
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::ReleaseIntArrayElements(jintArray array, jint *elems, jint mode) {
		blackbone::RemoteFunction<JavaDeclarations::java_ReleaseIntArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ReleaseIntArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			elems,
			mode
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::ReleaseLongArrayElements(jlongArray array, jlong *elems, jint mode) {
		blackbone::RemoteFunction<JavaDeclarations::java_ReleaseLongArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ReleaseLongArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			elems,
			mode
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::ReleaseFloatArrayElements(jfloatArray array, jfloat *elems, jint mode) {
		blackbone::RemoteFunction<JavaDeclarations::java_ReleaseFloatArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ReleaseFloatArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			elems,
			mode
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::ReleaseDoubleArrayElements(jdoubleArray array, jdouble *elems, jint mode) {
		blackbone::RemoteFunction<JavaDeclarations::java_ReleaseDoubleArrayElements> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ReleaseDoubleArrayElements) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			elems,
			mode
		};
		remoteFunction.Call(args, _WorkerThread);
	}

	void  JNI::GetBooleanArrayRegion(jbooleanArray array, jsize start, jsize l, jboolean *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetBooleanArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetBooleanArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			l,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::GetByteArrayRegion(jbyteArray array, jsize start, jsize len, jbyte *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetByteArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetByteArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::GetCharArrayRegion(jcharArray array, jsize start, jsize len, jchar *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetCharArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetCharArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::GetShortArrayRegion(jshortArray array, jsize start, jsize len, jshort *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetShortArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetShortArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::GetIntArrayRegion(jintArray array, jsize start, jsize len, jint *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetIntArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetIntArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::GetLongArrayRegion(jlongArray array, jsize start, jsize len, jlong *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetLongArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetLongArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::GetFloatArrayRegion(jfloatArray array, jsize start, jsize len, jfloat *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetFloatArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetFloatArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::GetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len, jdouble *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetDoubleArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetDoubleArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}

	void  JNI::SetBooleanArrayRegion(jbooleanArray array, jsize start, jsize l, const jboolean *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetBooleanArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetBooleanArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			l,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetByteArrayRegion(jbyteArray array, jsize start, jsize len, const jbyte *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetByteArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetByteArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetCharArrayRegion(jcharArray array, jsize start, jsize len, const jchar *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetCharArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetCharArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetShortArrayRegion(jshortArray array, jsize start, jsize len, const jshort *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetShortArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetShortArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetIntArrayRegion(jintArray array, jsize start, jsize len, const jint *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetIntArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetIntArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetLongArrayRegion(jlongArray array, jsize start, jsize len, const jlong *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetLongArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetLongArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetFloatArrayRegion(jfloatArray array, jsize start, jsize len, const jfloat *buf) {
		blackbone::RemoteFunction < JavaDeclarations::java_SetFloatArrayRegion > remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetFloatArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::SetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len, const jdouble *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_SetDoubleArrayRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.SetDoubleArrayRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}

	void  JNI::GetStringRegion(jstring str, jsize start, jsize len, jchar *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStringRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStringRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)str,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}
	void  JNI::GetStringUTFRegion(jstring str, jsize start, jsize len, char *buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStringUTFRegion> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStringUTFRegion) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)str,
			start,
			len,
			buf
		};
		remoteFunction.Call(args, _WorkerThread);
	}

	void *  JNI::GetPrimitiveArrayCritical(jarray array, jboolean *isCopy) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetPrimitiveArrayCritical> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetPrimitiveArrayCritical) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			isCopy
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	void  JNI::ReleasePrimitiveArrayCritical(jarray array, void *carray, jint mode) {
		blackbone::RemoteFunction<JavaDeclarations::java_ReleasePrimitiveArrayCritical> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ReleasePrimitiveArrayCritical) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)array,
			carray,
			mode
		};
		remoteFunction.Call(args, _WorkerThread);
	}

	const jchar *  JNI::GetStringCritical(jstring string, jboolean *isCopy) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetStringCritical> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetStringCritical) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)string,
			isCopy
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	void  JNI::ReleaseStringCritical(jstring string, const jchar *cstring) {
		blackbone::RemoteFunction<JavaDeclarations::java_ReleaseStringCritical> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ReleaseStringCritical) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)string,
			cstring
		};
		remoteFunction.Call(args, _WorkerThread);
	}

	jboolean JNI::ExceptionCheck() {
		blackbone::RemoteFunction<JavaDeclarations::java_ExceptionCheck> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.ExceptionCheck) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	jobject JNI::NewDirectByteBuffer(void* address, jlong capacity) {
		blackbone::RemoteFunction<JavaDeclarations::java_NewDirectByteBuffer> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.NewDirectByteBuffer) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			address,
			capacity
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	void*  JNI::GetDirectBufferAddress(jobject buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetDirectBufferAddress> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetDirectBufferAddress) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)buf
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}
	jlong JNI::GetDirectBufferCapacity(jobject buf) {
		blackbone::RemoteFunction<JavaDeclarations::java_GetDirectBufferCapacity> remoteFunction{ _InnerProcess, (blackbone::ptr_t)(envFuncs.GetDirectBufferCapacity) };
		decltype(remoteFunction)::CallArguments args{
			(void*)_ENV,
			(void*)buf
		};
		return remoteFunction.Call(args, _WorkerThread).result();
	}

	/* End JNIEnv Specific */

	/* Utilities for getting struct functions */

	JNIInvokeInterface_ JNI::GetInvokeInterface() {
		HANDLE pHandle = OpenProcess(PROCESS_VM_READ, TRUE, _InnerProcess.pid());
		JavaVM copyVM;
		if (!ReadProcessMemory(pHandle, (void*)_VM, &copyVM, sizeof(copyVM), 0)) {
			throw gcnew System::Exception("Failed to read target process memory at JVM location.");
		}
		JNIInvokeInterface_ copyII;
		if (!ReadProcessMemory(pHandle, (void*)copyVM.functions, &copyII, sizeof(copyII), 0)) {
			throw gcnew System::Exception("Failed to read target process memory at Invoke Interface location.");
		}
		return copyII;
	}

	JNINativeInterface_ JNI::GetNativeInterface() {
		HANDLE pHandle = OpenProcess(PROCESS_VM_READ, TRUE, _InnerProcess.pid());
		JNIEnv copyEnv;
		if (!ReadProcessMemory(pHandle, (void*)_ENV, &copyEnv, sizeof(copyEnv), 0)) {
			throw gcnew System::Exception("Failed to read target process memory at ENV location.");
		}
		JNINativeInterface_ copyNI;
		if (!ReadProcessMemory(pHandle, (void*)copyEnv.functions, &copyNI, sizeof(copyNI), 0)) {
			throw gcnew System::Exception("Failed to read target process memory at Native Interface location.");
		}
		return copyNI;
	}

	/* End struct utilities */
}