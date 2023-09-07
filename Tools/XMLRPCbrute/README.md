## WordPress XML-RPC Brute Force Amplification Attack

This script uses a vulnerability discovered in the XML-RPC implementation in WordPress to brute force user accounts. This allows for amplification of hundreds (or thousands) of requests per individual HTTP(s) request. For more details on the attack, see the related blog post on [sucuri.net](https://blog.sucuri.net/2015/10/brute-force-amplification-attacks-against-wordpress-xmlrpc.html).

NOTE: As of WordPress 4.4, this amplification method no longer works. All system.multicall requests via XML-RPC fail after the first authentication failure. See https://core.trac.wordpress.org/ticket/34336 for more details.

## Usage
```
$ ./wpxmlrpcbrute.py -c 1500 -u admin http://example.com/ wordlists.txt
```
