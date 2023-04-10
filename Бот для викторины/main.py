import os
import pyautogui
import random
import requests
from time import sleep
pyautogui.PAUSE = 0.25
pyautogui.FAILSAFE = True

layout = dict(zip(map(ord, "йцукенгшщзхъфывапролджэячсмитьбю.ё"
                           'ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ,Ё'),
                           "qwertyuiop[]asdfghjkl;'zxcvbnm,./`"
                           'QWERTYUIOP{}ASDFGHJKL:"ZXCVBNM<>?~'))

def contest(index,stroka):
    arr_otvet = []
    check = False
    if stroka.find('решивший', index) != -1:
        print('Contest')
        index_1 = stroka.find('решивший',index)
        index_1 = stroka.find(' ', index_1)+1
        index_2 = stroka.find(' ', index_1)
        num1 = int(stroka[index_1:index_2])

        index_1 = index_2+1
        index_2 = index_1+1
        sign = stroka[index_1:index_2]

        index_1 = index_2+1
        index_2 = stroka.find(',', index_1)
        num2 = int(stroka[index_1:index_2])


        if sign == '+':
            otvet = num1 + num2
        if sign == '-':
            otvet = num1 - num2
        arr_otvet.append(otvet)
        check = True

    if stroka.find('ответивший', index) != -1:
        index_1 = stroka.find('\n', index)+1
        index_2 = stroka.find('\n', index_1)
        str_temp = stroka[index_1 + 40:index_2]
        print(str_temp)
        url = f'https://baza-otvetov.ru/search/view/?q={str_temp.replace(" ", "+")}'
        print(url)
        text = requests.get(url).text
        index_1 = text.find('По вашему запросу ничего не найдено')
        if index_1 == -1:
            index_1 = text.find('<span>Ответ:')
            while text.find('<span>Ответ:', index_1) != -1:
                index_1 = text.find('<span>Ответ:', index_1)
                index_1 = text.find('</span>', index_1) + 7
                index_2 = text.find('<', index_1)
                arr_otvet.append(text[index_1:index_2])
            check = True
        else:
            check = False

    if check:
        return arr_otvet
    else:
        return 'no'


size_t = os.path.getsize('D:\mine\TechnoMagic\logs\latest.log')

f = open('D:\mine\TechnoMagic\logs\latest.log', mode='r')
print('monitoring')
while True:
    size = os.path.getsize('D:\mine\TechnoMagic\logs\latest.log')
    f.seek(size_t)
    if size != size_t:
        stroka = f.read(size-size_t)
        print(stroka)
        index = stroka.rfind('[Client thread/INFO]: [CHAT] [Викторина]')
        if index != -1:
            contesting = contest(index, stroka)
            if contesting == 'no':
                print('', end = '')
            else:
                tsleep = random.uniform(1.9, 3.7)
                print(f'Ответ: {contesting}')
                for i in contesting:
                    showchat = i
                    if type(showchat) != str:
                        showchat = str(showchat)
                        tsleep = random.uniform(3.2, 5.71)
                    print(tsleep)
                    showchat = showchat.translate(layout)
                    sleep(0.1)
                    pyautogui.press('T')
                    sleep(0.3)
                    pyautogui.typewrite(showchat, interval=0.02)
                    sleep(tsleep)
                    pyautogui.press('ENTER')
                    sleep(0.13)
                    tsleep = 0.3

                    size_t = size
                    size = os.path.getsize('D:\mine\TechnoMagic\logs\latest.log')
                    stroka = f.read(size-size_t)
                    showbreak_1 = stroka.find('[Client thread/INFO]: [CHAT] [Викторина] Игрок') != -1
                    showbreak_2 = stroka.find('Ожидайте следующей викторины!') != -1
                    if showbreak_1 and showbreak_2 and showbreak_1!= False and showbreak_2!= False:
                        print('Contest End')
                        break
        size_t = size
        sleep(0.5)