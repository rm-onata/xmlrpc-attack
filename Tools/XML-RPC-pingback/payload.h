#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <string.h>
#include <stdint.h>

char* xml_create_payload(const char* target_host, char* xml_host);
char* xml_http_request(attack_t* attack_data, struct node* reflector);

#endif // PAYLOAD_H
