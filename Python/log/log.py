# -*- coding: utf-8 -*-
import logging
import ctypes

color_blue = 0x01
color_green = 0x02
color_red = 0x04
color_white = 0x0007
color_yellow = color_red | color_green

std_output_handle = -11
std_out_handle = ctypes.windll.kernel32.GetStdHandle(std_output_handle)

def set_color(color, handle=std_out_handle):
    ret = ctypes.windll.kernel32.SetConsoleTextAttribute(handle, color)
    return ret

class Logger:
    def __init__(self, path, clevel=logging.DEBUG, flevel=logging.DEBUG):
        self.logger = logging.getLogger(path)
        self.logger.setLevel(logging.DEBUG)

        fmt = logging.Formatter('[%(asctime)s] [%(levelname)s] %(message)s', '%Y-%m-%d %H:%M:%S')
        # 设置控制台日志
        cmd = logging.StreamHandler()
        cmd.setFormatter(fmt)
        cmd.setLevel(clevel)

        # 设置文件日志
        file = logging.FileHandler(path)
        file.setFormatter(fmt)
        file.setLevel(flevel)

        self.logger.addHandler(cmd)
        self.logger.addHandler(file)

    def debug(self, message):
        self.logger.debug(message)

    def info(self, message):
        self.logger.info(message)

    def warning(self, message, color=color_yellow):
        set_color(color)
        self.logger.warning(message)
        set_color(color_white)

    def error(self, message, color=color_red):
        set_color(color)
        self.logger.error(message)
        set_color(color_white)

    def critical(self, message):
        self.logger.critical(message)

if __name__ == '__main__':
    log_test = Logger("test.log", logging.DEBUG, logging.WARNING)
    log_test.debug("debug information")
    log_test.info("info information")
    log_test.warning("warn information")
    log_test.error("error information")
    log_test.critical("critical information")