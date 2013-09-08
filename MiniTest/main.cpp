#include "jsapi.h"

/* The class of the global object */
static JSClass  global_class = { "global",
								JSCLASS_NEW_RESOLVE| JSCLASS_GLOBAL_FLAGS,
								JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
								JS_StrictPropertyStub, JS_EnumerateStub,
								JS_ResolveStub,JS_ConvertStub,NULL,
								JSCLASS_NO_OPTIONAL_MEMBERS};


void reportError(JSContext* cx, const char * message, JSErrorReport* report)
{
	fprintf(stderr, "%s:%u:%s\n",report->filename?report->filename:"[no filename]",
		(unsigned int)report->lineno, message);
}

JSBool  GetString(JSContext *cx, unsigned argc, JS::Value *vp)
{
	return  JS_FALSE;
}

int run(JSContext * cx)
{
	JSAutoRequest	ar(cx);

	JSObject * global = JS_NewGlobalObject(cx,&global_class,NULL);
	if(global == NULL)
		return 1;

	JSAutoCompartment  ac(cx,global);
	JS_SetGlobalObject(cx, global);

	if(!JS_InitStandardClasses(cx,global))
		return 1;

	JS_DefineFunction(cx,global,"GetString",GetString,0,0);
	return 0;
}
 


int main(int argc, const char *argv[])
{
	/* Create a JS runtime. */
	JSRuntime *rt = JS_NewRuntime(8L * 1024L * 1024L, JS_NO_HELPER_THREADS);
	if (rt == NULL)
		return 1;

	/* Create a context. */
	JSContext *cx = JS_NewContext(rt, 8192);
	if (cx == NULL)
		return 1;
	JS_SetOptions(cx, JSOPTION_VAROBJFIX);
	JS_SetErrorReporter(cx, reportError);

	int status = run(cx);

	JS_DestroyContext(cx);
	JS_DestroyRuntime(rt);

	/* Shut down the JS engine. */
	JS_ShutDown();

	return status;

}