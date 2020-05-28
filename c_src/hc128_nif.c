#include "erl_nif.h"


static ERL_NIF_TERM alloc_context_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    int x, ret;
    if (!enif_get_int(env, argv[0], &x)) {
	return enif_make_badarg(env);
    }
    ret = foo(x);
    return enif_make_int(env, ret);
}


static ErlNifFunc hc128_nif_funcs[] =
{
	{"alloc_context_nif", 1, alloc_context_nif},
	{"release_context_nif", 1, release_context_nif},
	{"list_context_nif", 0, list_context_nif},
    {"set_key_iv_nif", 3, set_key_iv_nif},
	{"decode_stream_nif", 3, decode_stream_nif, ERL_NIF_DIRTY_JOB_CPU_BOUND}
};

ERL_NIF_INIT(Elixir.HC128, hc128_nif_funcs, NULL, NULL, NULL, NULL)