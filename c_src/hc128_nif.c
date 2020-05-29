#include "erl_nif.h"
#include "hc128.h"

typedef struct {
  HC128 c;
  unsigned long created;
  ERL_NIF_TERM pid;
} context_t;

static ErlNifResourceType* CONTEXT_TYPE;
static ERL_NIF_TERM atom_ok;
static ERL_NIF_TERM atom_error;
static ERL_NIF_TERM atom_invalid_resource;
static ERL_NIF_TERM atom_invalid_key_iv;
static ERL_NIF_TERM atom_out_of_memory;
static ERL_NIF_TERM atom_decode_stream;

static ERL_NIF_TERM alloc_context_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    context_t* ctx = NULL;
    unsigned long created = 0;
    ERL_NIF_TERM ret;
    if(argc != 2) {
        return enif_make_badarg(env);
    }
    if(!enif_is_pid(env, argv[0])) {
        return enif_make_badarg(env);
    }
    if(!enif_get_ulong(env, argv[1], &created)) {
        return enif_make_badarg(env);
    }
    ctx = (context_t*)enif_alloc_resource(CONTEXT_TYPE, sizeof(context_t));
    ctx->created = created;
    ctx->pid = argv[0];
    ret = enif_make_resource(env, ctx);
    // Let Elixir gc
    enif_release_resource(ctx);
    // Increase tracking counter
    int* context_num = (int*)enif_priv_data(env);
    (*context_num)++;
    return enif_make_tuple2(env, atom_ok, ret);
}

static ERL_NIF_TERM count_context_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    int* context_num = (int*)enif_priv_data(env);
    return enif_make_int(env, (*context_num));
}

static ERL_NIF_TERM print_context_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    context_t *ctx = NULL;
    ERL_NIF_TERM created;
    if(argc != 1) {
        return enif_make_badarg(env);
    }
    if(!enif_get_resource(env, argv[0], CONTEXT_TYPE, (void**)&ctx) || ctx == NULL) {
        return enif_make_tuple2(env, atom_error, atom_invalid_resource);
    }
    created = enif_make_ulong(env, ctx->created);
    return enif_make_tuple2(env, ctx->pid, created);
}

static ERL_NIF_TERM set_key_iv_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    context_t *ctx = NULL;
    ErlNifBinary key;
    ErlNifBinary iv;
    if(argc != 3) {
        return enif_make_badarg(env);
    }
    if(!enif_is_binary(env, argv[1]) || !enif_is_binary(env, argv[2])) {
        return enif_make_badarg(env);
    }
    if(!enif_inspect_binary(env, argv[1], &key) || !enif_inspect_binary(env, argv[2], &iv)) {
        return enif_make_badarg(env);
    }
    if(!enif_get_resource(env, argv[0], CONTEXT_TYPE, (void**)&ctx) || ctx == NULL) {
        return enif_make_tuple2(env, atom_error, atom_invalid_resource);
    }
    if(wc_Hc128_SetKey(&ctx->c, key.data, iv.data) == 0) {
        return atom_ok;
    } else {
        return enif_make_tuple2(env, atom_error, atom_invalid_key_iv);
    }
}

static ERL_NIF_TERM decode_stream_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    context_t *ctx = NULL;
    ErlNifBinary input;
    ErlNifBinary output;
    ERL_NIF_TERM ret;
    if(argc != 2) {
        return enif_make_badarg(env);
    }
    if(!enif_is_binary(env, argv[1]) || !enif_inspect_binary(env, argv[1], &input)) {
        return enif_make_badarg(env);
    }
    if(!enif_get_resource(env, argv[0], CONTEXT_TYPE, (void**)&ctx) || ctx == NULL) {
        return enif_make_tuple2(env, atom_error, atom_invalid_resource);
    }
    if(!enif_alloc_binary(input.size, &output)) {
        return enif_make_tuple2(env, atom_error, atom_out_of_memory);
    }
    if(wc_Hc128_Process(&ctx->c, output.data, input.data, input.size) == 0) {
        ret = enif_make_binary(env, &output);
        enif_release_binary(&output);
        return enif_make_tuple2(env, atom_ok, ret);;
    } else {
        enif_release_binary(&output);
        return enif_make_tuple2(env, atom_error, atom_decode_stream);
    }
}

static void free_context_resource(ErlNifEnv* env, void* obj) {
    int* context_num = (int*)enif_priv_data(env);
    (*context_num)--;
}

static inline int init_context_resource(ErlNifEnv* env)
{
    const char* mod = "HC128";
    const char* name = "Context";
    int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;

    CONTEXT_TYPE = enif_open_resource_type(env, mod, name, free_context_resource, (ErlNifResourceFlags)flags, NULL);
    if(CONTEXT_TYPE == NULL) return -1;
    return 0;
}


static int init_nif(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info) {
    if (init_context_resource(env) == -1) {
        return -1;
    }
    int* context_num = (int*)enif_alloc(sizeof(int ));
    (*context_num) = 0;
    *priv_data = (void*)context_num;
    atom_ok = enif_make_atom(env, "ok");
    atom_error = enif_make_atom(env, "error");
    atom_invalid_resource = enif_make_atom(env, "invalid_resource");
    atom_invalid_key_iv = enif_make_atom(env, "invalid_key_iv");
    atom_out_of_memory = enif_make_atom(env, "out_of_memory");
    atom_decode_stream = enif_make_atom(env, "decode_stream");
    return 0;
}

static void destroy_inf(ErlNifEnv* env, void* priv_data) {
    if(priv_data) {
        enif_free(priv_data);
    }
}

static ErlNifFunc hc128_nif_funcs[] =
{
	{"alloc_context_nif", 2, alloc_context_nif},
    {"print_context_nif", 1, print_context_nif},
	{"count_context_nif", 0, count_context_nif},
    {"set_key_iv_nif", 3, set_key_iv_nif, ERL_NIF_DIRTY_JOB_CPU_BOUND},
	{"decode_stream_nif", 2, decode_stream_nif, ERL_NIF_DIRTY_JOB_CPU_BOUND}
};

ERL_NIF_INIT(Elixir.HC128, hc128_nif_funcs, init_nif, NULL, NULL, destroy_inf)