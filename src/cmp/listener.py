#!/usr/bin/python


import sys
import pynput

class KeyboardListener:
    def __init__(self, join=False, key_down_callback=lambda self, key: None, key_up_callback=lambda self, key: None):
        self.previous_keys_down = set()
        self.keys_down = set()
        self.key_down_callback = key_down_callback
        self.key_up_callback = key_up_callback

        def reportKeyUp(key):
            try: k = key.char # single-char keys
            except: k = key.name # other keys
            self.removeKey(k)

            self.key_up_callback(self, k) # self is explicitly given as an argument so that the callback can access the instance

            return True # Returning true so that the pynput event dispatcher continues to send key events

        def reportKeyDown(key):
            try: k = key.char # single-char keys
            except: k = key.name # other keys
            self.addKey(k)

            self.key_down_callback(self, k) # self is explicitly given as an argument so that the callback can access the instance

            return True # Returning true so that the pynput event dispatcher continues to send key events


        self.listener = pynput.keyboard.Listener(on_press=reportKeyDown, on_release=reportKeyUp)
        self.listener.start()

        if join:
            self.listener.join()

    def addKey(self, key):
        self.keys_down.add(key)

    def removeKey(self, key):
        try:
            self.keys_down.remove(key)
        except:
            pass


    def __str__(self):
        return str(self.keys_down)
