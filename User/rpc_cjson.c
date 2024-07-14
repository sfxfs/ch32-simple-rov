#include "rpc_cjson.h"

#include "debug.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief ��ӻص�����
 * @param handle rpc ���
 * @param function_pointer �ص�������ָ��
 * @param name �ص��ķ�����
 * @param data ����ص�����������
 * @return �ɹ����� 0��ʧ�ܷ��� -1
 */
int rpc_add_method(rpc_handle_t *handle, jrpc_function function_pointer, char *name, void *data)
{
    int i = handle->procedure_count++;
    if (!handle->procedures)
        handle->procedures = malloc(sizeof(struct jrpc_procedure));
    else
    {
        struct jrpc_procedure *ptr = realloc(handle->procedures,
                                             sizeof(struct jrpc_procedure) * handle->procedure_count);
        if (!ptr)
        {
            printf("Cannot alloc mem for func %s\r\n", name);
            return -1;
        }
        handle->procedures = ptr;
    }
    if ((handle->procedures[i].name = strdup(name)) == NULL)
        return -1;
    handle->procedures[i].function = function_pointer;
    handle->procedures[i].data = data;
    return 0;
}

/**
 * @brief ���� procedure �ṹ��
 * @param procedure �ṹ��ָ��
 */
static void procedure_destroy(struct jrpc_procedure *procedure)
{
    if (procedure->name)
    {
        free(procedure->name);
        procedure->name = NULL;
    }
    if (procedure->data)
    {
        free(procedure->data);
        procedure->data = NULL;
    }
}

/**
 * @brief ɾ���ص�����
 * @param handle rpc ���
 * @param name ������
 * @return �ɹ����� 0��ʧ�ܷ��� -1
 */
int rpc_del_method(rpc_handle_t *handle, char *name)
{
    int i;
    int found = 0;
    if (handle->procedures)
    {
        for (i = 0; i < handle->procedure_count; i++)
        {
            if (found)
                handle->procedures[i - 1] = handle->procedures[i];
            else if (!strcmp(name, handle->procedures[i].name))
            {
                found = 1;
                procedure_destroy(&(handle->procedures[i]));
            }
        }
        if (found)
        {
            handle->procedure_count--;
            if (handle->procedure_count)
            {
                struct jrpc_procedure *ptr = realloc(handle->procedures,
                                                     sizeof(struct jrpc_procedure) * handle->procedure_count);
                if (!ptr)
                {
                    printf("realloc\r\n");
                    return -1;
                }
                handle->procedures = ptr;
            }
            else
            {
                handle->procedures = NULL;
            }
        }
    }
    else
    {
        printf("Method '%s' not found.\r\n", name);
        return -1;
    }
    return 0;
}

/**
 * @brief ����ɹ����ûص����������ݰ�
 * @param result �ص������ķ���
 * @param id ����� id ��
 * @return ���ع���������ݰ�
 */
static cJSON *rpc_ok(cJSON *result, cJSON *id)
{
    cJSON *result_root = cJSON_CreateObject();

    cJSON_AddStringToObject(result_root, "jsonrpc", JRPC_VERSION);
    cJSON_AddItemToObject(result_root, "result", result);
    if (id)
        cJSON_AddItemToObject(result_root, "id", id);
    else
        cJSON_AddItemToObject(result_root, "id", cJSON_CreateNull());

    return result_root;
}

/**
 * @brief ����ʧ�ܵ��ûص����������ݰ�
 * @param code �������
 * @param message ������Ϣ
 * @param id ����� id ��
 * @return ���ع���������ݰ�
 */
static cJSON *rpc_err(rpc_handle_t *handle, int code, char *message, cJSON *id)
{
    cJSON *result_root = cJSON_CreateObject();
    cJSON *error_root = cJSON_CreateObject();
    if (result_root == NULL || error_root == NULL)
        return NULL;

    cJSON_AddNumberToObject(error_root, "code", code);
    if (message)
    {
        if (handle->message.total++ == 0)
            handle->message.str_array = malloc(handle->message.total * sizeof(char *));
        else
            handle->message.str_array = realloc(handle->message.str_array, handle->message.total * sizeof(char *));
        handle->message.str_array[handle->message.cur++] = message;

        cJSON_AddStringToObject(error_root, "message", message);
    }

    cJSON_AddStringToObject(result_root, "jsonrpc", JRPC_VERSION);
    cJSON_AddItemToObject(result_root, "error", error_root);
    if (id)
        cJSON_AddItemToObject(result_root, "id", id);
    else
        cJSON_AddItemToObject(result_root, "id", cJSON_CreateNull());

    return result_root;
}

/**
 * @brief ���ûص�����
 * @param handle rpc ���
 * @param name �ص�����������
 * @param params �ص���������
 * @param id ����� id ��
 * @return ���ص��õĽ��
 */
static cJSON *invoke_procedure(rpc_handle_t *handle, char *name, cJSON *params, cJSON *id)
{
    cJSON *returned = NULL;
    int procedure_found = 0;
    jrpc_context ctx;
    ctx.error_code = 0;
    ctx.error_message = NULL;
    int i = handle->procedure_count;
    while (i--)
    {
        if (!strcmp(handle->procedures[i].name, name))
        {
            procedure_found = 1;
            ctx.data = handle->procedures[i].data;
            returned = handle->procedures[i].function(&ctx, params, id);
            break;
        }
    }
    if (!procedure_found)
    {
        printf("Method '%s' not found.\r\n", name);
        return rpc_err(handle, JRPC_METHOD_NOT_FOUND, strdup("Method not found."), id);
    }
    else
    {
        if (ctx.error_code)
            return rpc_err(handle, ctx.error_code, ctx.error_message, id);
        else
            return rpc_ok(returned, id);
    }
}

/**
 * @brief ���ûص�����
 * @param handle rpc ���
 * @param request rpc �����������ݰ�
 * @return ���ص��õĽ��
 */
static cJSON *rpc_invoke_method(rpc_handle_t *handle, cJSON *request)
{
    cJSON *method, *params, *id;
    if (strcmp("2.0", cJSON_GetObjectItem(request, "jsonrpc")->valuestring) != 0)
        return rpc_err(handle, JRPC_INVALID_REQUEST, strdup("Valid request received: JSONRPC version error."), NULL);
    method = cJSON_GetObjectItem(request, "method");
    if (method != NULL && method->type == cJSON_String)
    {
        params = cJSON_GetObjectItem(request, "params");
        id = cJSON_GetObjectItem(request, "id");
        if (id->type == cJSON_NULL || id->type == cJSON_String || id->type == cJSON_Number)
        {
            // We have to copy ID because using it on the reply and deleting the response Object will also delete ID
            cJSON *id_copy = NULL;
            if (id != NULL)
                id_copy =
                    (id->type == cJSON_String) ? cJSON_CreateString(
                                                     id->valuestring)
                                               : cJSON_CreateNumber(id->valueint);
            printf("Method Invoked: %s.\r\n", method->valuestring);
            return invoke_procedure(handle, method->valuestring, params, id_copy);
        }
        return rpc_err(handle, JRPC_INVALID_REQUEST, strdup("Valid request received: No 'id' member"), NULL);
    }
    return rpc_err(handle, JRPC_INVALID_REQUEST, strdup("Valid request received: No 'method' member"), NULL);
}

/**
 * @brief ���ûص�������������ʽ��
 * @param handle rpc ���
 * @param request rpc �����������ݰ�
 * @return ���ص��õĽ��
 */
static cJSON *rpc_invoke_method_array(rpc_handle_t *handle, cJSON *request)
{
    int array_size = cJSON_GetArraySize(request);
    if (array_size <= 0)
        return rpc_err(handle, JRPC_INVALID_REQUEST, strdup("Valid request received: Empty JSON array."), NULL);

    cJSON *return_json_array = cJSON_CreateArray();
    for (int i = 0; i < array_size; i++)
        cJSON_AddItemToArray(return_json_array, rpc_invoke_method(handle, cJSON_GetArrayItem(request, i)));

    return return_json_array;
}

/**
 * @brief jsonrpc ������
 * @param handle rpc ���
 * @param json_req json ��ʽ����������
 * @param req_len �������ݵĳ���
 * @return �������������ַ���
 */
char *rpc_process(rpc_handle_t *handle, const char *json_req, size_t req_len)
{
    char *str_return = NULL;

    if (json_req == NULL || req_len <= 0)
    {
        printf("Empty Request Received.\r\n");
        str_return = cJSON_PrintUnformatted(rpc_err(handle, JRPC_PARSE_ERROR, strdup("Parse error: Not in JSON format."), NULL));
        goto req_error;
    }

//    printf("Recv json str:\n%s\r\n", json_req);

    cJSON *request = cJSON_Parse(json_req);
    if (request == NULL)
    {
        printf("Valid JSON Received.\r\n");
        str_return = cJSON_PrintUnformatted(rpc_err(handle, JRPC_PARSE_ERROR, strdup("Parse error: Not in JSON format."), NULL));
        goto req_error;
    }

    cJSON *cjson_return = NULL;
    if (request->type == cJSON_Array)
    {
        cjson_return = rpc_invoke_method_array(handle, request);
    }
    else if (request->type == cJSON_Object)
    {
        cjson_return = rpc_invoke_method(handle, request);
    }
    else
    {
        cjson_return = rpc_err(handle, JRPC_INVALID_REQUEST, strdup("Valid request received: Not a JSON object or array."), NULL);
    }

    str_return = cJSON_PrintUnformatted(cjson_return);
    cJSON_Delete(cjson_return);
    cJSON_Delete(request);

req_error:
    if (handle->message.total != 0)
    {
        for (int i = 0; i < handle->message.total; i++)
            free(handle->message.str_array[i]);
        free(handle->message.str_array);
        memset(&handle->message, 0, sizeof(str_free_t));
    }
    printf("return json str:\n%s\r\n", str_return);
    return str_return;
}
