//% gcc -o GPSJson GPSJson.c -I/opt/homebrew/Cellar/json-c/0.16/include
//gcc -o GPSJson GPSJson.c -I/opt/homebrew/Cellar/json-c/0.16/include -I/Library/Developer/CommandLineTools/SDKs/MacOSX13.3.sdk/usr/include 
#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <string.h>

#define API_URL "https://ipinfo.io/json"

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        printf("Failed to allocate memory\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main() {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, API_URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "Failed to get location information: %s\n", curl_easy_strerror(res));
        } else {
            json_object *json = json_tokener_parse(chunk.memory);
            json_object *loc = json_object_object_get(json, "loc");

            printf("Your location coordinates are: %s\n", json_object_get_string(loc));
        }

        curl_easy_cleanup(curl);
    }

    free(chunk.memory);

    return 0;
}
