import sublime
import sublime_plugin
from math import *
from random import *
from time import time

rr = randrange
fact = factorial

timeout = 3

def gcd(a, b):
    if a > b:
        a, b = b, a
    if a == 0:
        return b
    return gcd(b % a, a)

def c(n, k):
    return fact(n) // (fact(k) * fact(n - k))

def factor(n):
    start_time = time()
    if n <= 1:
        return str(n)
    p = 3
    answer = []
    while n % 2 == 0:
        answer.append("2")
        n //= 2
    while p * p <= n:
        while n % p == 0:
            n //= p
            answer.append(str(p))
        p += 2
        if time() > start_time + timeout:
            if n != 1:
                answer.append("({0})".format(n))
                n = 1
                break
    if n != 1:
        answer.append(str(n))
    return ' * '.join([p for p in answer])

def primes_between(a, b):
    start_time = time()
    is_prime = [True for i in range(b - a + 1)]
    answer = []
    for i in range(2, int(sqrt(b)) + 5):
        j = a // i * i
        while j < a:
            j += i
        while j <= b:
            is_prime[j - a] = False
            j += i
        if time() > start_time + timeout:
            answer.append(-1)
            break
    for i in range(b - a + 1):
        if is_prime[i]:
            answer.append(a + i)
    return answer

# [name, parameters]
commands = [["factor", 1], ["c", 2], ["fact", 1], ["sin", 1], ["cos", 1], ["rr", 2], ["primes_between", 2],\
    ["log", 2], ["gcd", 2]]

class InsertResultCommand(sublime_plugin.TextCommand):
    def run(self, edit, **args):
        if args.get("position") is None:
            position = self.view.sel()[0].begin()
        else:
            position = args["position"]
        self.view.insert(edit, position, args["text"])
        self.view.sel().add(sublime.Region(position, position + len(args["text"])))

class CalculateCommand(sublime_plugin.TextCommand):
    def run(self, edit):
        if len(self.view.sel()) == 1 and self.view.sel()[0].empty():
            self.view.window().show_input_panel("", "", self.done, None, None)
        else:
            for region in list(self.view.sel())[::-1]:
                text = self.view.substr(region)
                result = str(self.calculate(text))
                if text[0] == '\n':
                    result = '\n' + result
                if text[-1] == '\n':
                    result = result + '\n'
                position = region.begin()
                self.view.erase(edit, region)
                self.view.run_command("insert_result", args={"text": result, "position": position})

    def calculate(delf, text):
        for command in commands:
            if text.startswith(command[0]) and text[len(command[0])] != '(':
                if command[1] > 1:
                    text = text[:len(command[0])] + '(' + text[len(command[0]) + 1:].replace(' ', ',') + ')'
                else:
                    text = text[:len(command[0])] + '(' + text[len(command[0]) + 1:] + ')'
                break
        return eval(text)

    def done(self, text):
        self.view.run_command("insert_result", args={"text": str(self.calculate(text))})
