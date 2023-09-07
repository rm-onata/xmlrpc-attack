from sys import exit, argv
from requests import get
from datetime import datetime
from functools import wraps


def current_time():
    time = datetime.now()
    hour = time.hour
    minute = time.minute
    second = time.second

    return f"[{hour}:{minute}:{second}]"


def banner(func):
    @wraps(func)
    def banner(*args):
        print("__          _________   __      _       _ _")
        print("\\ \\        / /  __ \\ \\ / /     | |     (_) |")
        print(" \\ \\  /\\  / /| |__) \\ V / _ __ | | ___  _| |_")
        print("  \\ \\/  \\/ / |  ___/ > < | '_ \\| |/ _ \\| | __|")
        print("   \\  /\\  /  | |    / . \\| |_) | | (_) | | |_")
        print("    \\/  \\/   |_|   /_/ \\_\\ .__/|_|\\___/|_|\\__|")
        print("                         | |   [Version 1.0.0]")
        print("                         |_|\n")
        return func(*args)
    return banner


def url_check(func):
    @wraps(func)
    def check(*args):
        url = args[0]
        if not (url.startswith("https://") or url.startswith("http://")):
            print(current_time(), "your url seems to be invalid.")
            exit(1)
        print(current_time(), "URL is valid!")
        return func(*args)
    return check


def connection_check(func):
    @wraps(func)
    def check(*args):
        try:
            url = args[0]
            con = get(url, timeout=5, headers={"user-agent": "mozilla"})
            con.close()
        except Exception:
            print(current_time(), "unable to connect to the target website.")
        else:
            print(current_time(), "connection is successfully establised!")
            return func(*args)
    return check


@banner
def help():
    print("[+] Usage\t: {} <url> <thread> <timeout>".format(argv[0]))
    print("[+] Examlple\t: {} http://target.com/ 5 15".format(argv[0]))
    exit(0)


def get_user_name():
    try:
        print(current_time(), "please provide the username : ", end="")
        user = input()
    except KeyboardInterrupt:
        exit(1)

    return user
