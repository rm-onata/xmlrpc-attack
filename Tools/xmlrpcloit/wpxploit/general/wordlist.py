import os
from wpxploit.general.interface import current_time


def show_word_list():

    DIR = "wordlist/"  # default dir of wordlist

    def filter_dir(element):
        """ eliminate directory in file list """

        return os.path.isfile(DIR + element)

    list_file = os.listdir(DIR)
    list_file = list(filter(filter_dir, list_file))
    list_file = list(map(lambda file: DIR + file, list_file))

    try:
        for count in range(len(list_file)):
            content = list_file[count]
            print(current_time(), "{}. {}".format(str(count), content))
        else:
            print(current_time(), "select your file number : ", end="")
            user_input = int(input())
    except (KeyboardInterrupt, ValueError):
        os._exit(1)

    return list_file[user_input]


def read_word_list(file_name: str, size: str) -> list:
    """
    generator for creating wordlist chunk
    """

    with open(file_name) as file:
        word_char = file.readlines().__iter__()
        word_size = word_char.__length_hint__() // size

        # put the stream point in the beginning of the file
        file.seek(0)

        while word_char.__length_hint__() != 0:
            chunk = []
            stops = False
            for word in range(word_size):
                try:
                    chunk.append(word_char.__next__().strip())
                except StopIteration:
                    stops = True
                    break

            yield chunk
