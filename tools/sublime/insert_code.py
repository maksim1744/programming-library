import sublime
import sublime_plugin
import os


path = "C:/Libs/"
ifdef_string = "#ifdef HOME"
int_main_string = "int main("
test_case_string = "void test_case("

class InsertFromFileCommand(sublime_plugin.TextCommand):
    def run(self, edit, **args):
        lines = open(args["file"], "r").readlines()
        curtext = self.view.substr(sublime.Region(0, self.view.size()))
        after_string = int_main_string
        if test_case_string in curtext:
            after_string = test_case_string
        if after_string in curtext:
            self.view.insert(edit, curtext.index(after_string), "".join(lines) + '\n')
        else:
            self.view.insert(edit, self.view.sel()[0].begin(), "".join(lines) + '\n')

class InsertTemplateCommand(sublime_plugin.TextCommand):
    def run(self, edit):
        self.files = list(filter(lambda file: ".exe" not in file, list(os.walk(path))[0][2]))
        self.view.window().show_quick_panel([file[:-4] for file in self.files], self.on_file_choose)

    def on_file_choose(self, index):
        if index == -1:
            return
        self.view.run_command("insert_from_file", {"file": path + self.files[index]})

class OpenTemplateCommand(sublime_plugin.TextCommand):
    def run(self, edit):
        self.files = list(filter(lambda file: ".exe" not in file, list(os.walk(path))[0][2]))
        self.view.window().show_quick_panel([file[:-4] for file in self.files], self.on_file_choose)

    def on_file_choose(self, index):
        if index == -1:
            return
        sublime.active_window().open_file(path + self.files[index])
