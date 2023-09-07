import requests


class XmlrpcDoesNotExist(Exception):
    """ raised when xmlrpc.php does not exist on target website """

    pass


class XmlrpcRequester:

    _user_agent = "Googlebot/2.1 (+http://www.google.com/bot.html)"

    def __init__(self, url: str, timeout: int = 10):
        self.url = url
        self.timeout = timeout

    # Protected Method
    # Should not be accessed outside the class
    @property
    def _get_status(self) -> bool:
        """ check xmlrpc existence """

        url = self.url + "/xmlrpc.php"
        header = {"user-agent": self._user_agent}
        timeout = self.timeout

        with requests.get(url, timeout=timeout, headers=header) as request:
            if request.status_code != 404:
                if "XML-RPC server accepts POST requests" in request.text:
                    return True
                else:
                    return False
            else:
                return False

    # Protected Methods
    # Should not be accessed outside the class
    def _make_xmlrpc_request(self, post_data: str) -> list:
        """ create request to xmlrpc.php on target website """

        url = self.url + "/xmlrpc.php"
        header = {"user-agent": self._user_agent}
        timeout = self.timeout

        request = requests.post(url=url, headers=header,
                                data=post_data,
                                timeout=timeout)

        if request.status_code != 404:
            if "faultCode" not in request.text:
                # request successfull (payload executed without error return)
                return_data = request.text
            else:
                # request unsuccessfull (payload executed with error return)
                return_data = None
        else:
            return_data = None

        request.close()
        return return_data
