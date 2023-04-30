# -*- coding: utf-8 -*-
import re
import ctypes
import logging
from logging.handlers import TimedRotatingFileHandler

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
        logging.basicConfig(encoding="uft8")
        self.logger = logging.getLogger()
        self.logger.setLevel(logging.DEBUG)

        fmt = logging.Formatter('[%(asctime)s] [%(levelname)s] %(message)s')

        # 设置控制台日志
        cmd = logging.StreamHandler()
        cmd.setFormatter(fmt)
        cmd.setLevel(clevel)

        # 设置文件日志，按时间滚动
        file = TimedRotatingFileHandler(path, when='D', interval=1, backupCount=5)
        file.suffix="%Y-%m-%d_%H-%M-%S.log"
        file.extMatch = re.compile(r"^\d{4}-\d{2}-\d{2}_-\d{2}-\d{2}-\d{2}.log$")
        file.setLevel(flevel)
        file.setFormatter(fmt)
        
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
    log_test = Logger("log_test", logging.DEBUG, logging.WARNING)
    log_test.debug("debug information")
    log_test.info("info information")
    log_test.warning("warn information")
    log_test.error("error information")
    log_test.critical("critical information")