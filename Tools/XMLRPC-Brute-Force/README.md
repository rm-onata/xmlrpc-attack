##About
This is an exploit for Wordpress xmlrpc.php System Multicall function affecting the most current version of Wordpress (3.5.1). The exploit works by sending 1,000+ auth attempts per request to xmlrpc.php in order to "brute force" valid Wordpress users and will iterate through whole wordlists until a valid user response is acquired. It will then selectively acquire and display the valid username and password to login.


## Usage
```
./wp-xml-brute http://target.com/xmlrpc.php passwords.txt username1 [username2] [username3]...
```

## reference
https://github.com/1N3/Wordpress-XMLRPC-Brute-Force-Exploit
