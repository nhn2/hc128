#include "erl_nif.h"
#include <map>
#include <string>

static std::map<std::string, ERL_NIF_TERM> context_list;

static ERL_NIF_TERM alloc_context_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, 0);
}

static ERL_NIF_TERM release_context_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, 0);
}

static ERL_NIF_TERM list_context_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, 0);
}


static ERL_NIF_TERM set_key_iv_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, 0);
}

static ERL_NIF_TERM decode_stream_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, 0);
}


static ErlNifFunc hc128_nif_funcs[] =
{
	{"alloc_context_nif", 1, alloc_context_nif},
	{"release_context_nif", 1, release_context_nif},
	{"list_context_nif", 0, list_context_nif},
    {"set_key_iv_nif", 3, set_key_iv_nif, ERL_NIF_DIRTY_JOB_CPU_BOUND},
	{"decode_stream_nif", 2, decode_stream_nif, ERL_NIF_DIRTY_JOB_CPU_BOUND}
};

ERL_NIF_INIT(Elixir.HC128, hc128_nif_funcs, NULL, NULL, NULL, NULL)