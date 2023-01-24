import os
import numpy as np
import pandas as pd
from Database import sqlConnection as DBsqllite


from kivy.clock import Clock
from kivy.properties import ObjectProperty, NumericProperty, BoundedNumericProperty, StringProperty
from kivy.lang import Builder
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.core.window import Window

from kivymd.app import MDApp
from kivymd.toast import toast
from kivymd.uix.button import MDIconButton, MDFillRoundFlatButton
from kivymd.uix.dialog import MDDialog
from kivymd.uix.menu import MDDropdownMenu

from kivy.config import Config

from kivy.app import App
from kivy.clock import Clock
from kivy.properties import NumericProperty
from kivy.properties import StringProperty
from kivy.properties import BoundedNumericProperty
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.widget import Widget
from kivy.uix.scatter import Scatter
from kivy.uix.image import Image
from kivy.uix.label import Label
from kivy.uix.progressbar import ProgressBar
from os.path import join, dirname, abspath
from kivy.uix.screenmanager import Screen, ScreenManager
from kivy.core.window import Window
from kivy.app import App
from kivymd.uix.datatables import MDDataTable
from kivy.metrics import dp
import matplotlib.pyplot as plt


DB = DBsqllite()

encodings = {
            #BMS_Pack
            10: "PackCurrent",
            11: "PackVoltage",
            12: "PackStateOfHealth",
            13: "AvgPackCurrent",
            14: "PackStateOfCharge",
            #BMS_Cell
            15: "LowCellVoltage",
            16: "HighCellVoltage",
            17: "AvgCellVoltage",
            #BMS_Failsafes
            18: "VoltageFailsafeActive",
            19: "CurrentFailsafeActive",
            20: "RelayFailsafeActive",
            21: "CellBalancingActive",
            22: "ChangeinterlockFailsafeActive",
            23: "ThermistorB_valueTableInvalid",
            24: "InputPowerSupplyFailed",
            #BMS_Temperatures
            25: "HighestTemperature",
            26: "LowestTemperature",
            27: "AverageTemperature",
            28: "InternalTemperature",

            #MC_Temperatures
            29: "HeatsinkTemperature",
            30: "MotorTemperature",
            #MC_CurrentVoltage
            31: "BusCurrent",
            32: "BusVoltage",
            #MC_Velocity
            33: "MotorVelocity",
            34: "VehicleVelocity",
            #MC_ErrorFlags
            35: "MotorOverSpeed",
            36: "DesaturationFault",
            37: "RailUnderVoltage",
            38: "ConfigReadError",
            39: "WatchdogCausedLastReset",
            40: "BadMotorPositionHallSequence",
            41: "DCBusOverVoltage",
            42: "SoftwareOverCurrent",
            43: "HardwareOverCurrent",
            #MC_LimitFlags
            44: "IPMTemperatureOrMotorTemperature",
            45: "BusVoltageLowerLimit",
            46: "BusVoltageUpperLimit",
            47: "BusCurrentLimit",
            48: "Velocity",
            49: "MotorCurrent",
            50: "OutputVoltagePWM"
        }


current_theme_light = False

class MainScreen(Screen):
    dialog = None


    def __init__(self, **kw):
        super().__init__(**kw)
        self.USB_connected = "False"
        self.connection_established = "False"


    def on_pre_enter(self):
        self.values_clock = Clock.schedule_interval(lambda update_values: self.update_data(), 1)

    def on_pre_leave(self):
        self.values_clock.cancel()


    # For switching between light- and dark -mode
    def switch_theme_mode(self):
        global current_theme_light
        if current_theme_light:
            current_theme_light = False
            self.lightMode.icon = "weather-night"
            self.lightMode.text = "Darkmode"
            self.lightMode.text_color = "white"
           # self.lightMode.icon_color = "white"
        else:
            current_theme_light = True
            self.lightMode.icon = "weather-sunny"
            self.lightMode.text = "Lightmode"
            self.lightMode.text_color = "black"
            #self.lightMode.icon_color = "black"

        self.update_color_theme()

    # Updates buttons and labels to
    # corresponding theme light/dark
    def update_color_theme(self):
        if current_theme_light:
            color = (0,0,0)
        else: 
            color = (1,1,1)

        for i in range(10, 35):
            # 17 < i < 25 are not affected by darkmode/lightmode
            if 17 < i < 25: 
                pass
            else:
                id = encodings[i][0].lower() + encodings[i][1:]
                exec("self." + id + ".color =" + str(color))

        
        # BUTTONS
        self.loraStatusBTN.text_color  = color
        self.dataDetailsBTN.text_color = color
        self.goodLabel.text_color = color
        self.badLabel.text_color = color

        # LABELS
        self.bmsPack.color = color
        self.bmsCell.color = color
        self.bmsFailsafes.color = color
        self.bmsTemperatures.color = color

        self.mcCurrentVoltage.color = color
        self.mcTemperatures.color = color
        self.mcVelocity.color = color
        self.mcErrorFlags.color = color
        self.mcLimitFlags.color = color


    def update_data(self):
        for i in range(10, 51):
            value = DB.execCommmand("SELECT * FROM " + encodings[i] + " ORDER BY ID DESC LIMIT 1")
            Id = encodings[i][0].lower() + encodings[i][1:]
            
            # Change text value
            if self.within_limits_float(i):
                
                if value != None:
                    #For ID from ex: BusCurrent -> busCurrent
                    #setattr(self, Id, None)
                    exec("self."+Id+".text='"+ encodings[i]+ ": " + str(round(value[0][2], 2))+"'")

            #Change color value True(Blue) / False(Red)
            else:
                if value[0][2] == 1:
                    color = (176/255, 18/255, 0/255)
                    exec("self."+Id+".color= color")
                else:
                    color = (0/255, 109/255, 176/255)
                    exec("self."+Id+".color= color")

                

    def within_limits_float(self, index):
        if 10 <= index <= 17 or 25 <= index <= 34:
            return True
        else:
            return False
        


    def show_lora_status(self):
        if not self.dialog:
            self.dialog = MDDialog(
                title = "LoRa Status",
                text = "USB-connected: " + self.USB_connected + "\n" + "Connection established: " + self.connection_established,
                buttons = [
                    MDFillRoundFlatButton(
                        text = "OK",
                        on_release = self.close_lora_status
                    ),
                ]
            )

        self.dialog.open()

    def close_lora_status(self, obj):
        self.dialog.dismiss()

#----------------------------------------------------DATA DETAILS SCREEN--------------------------------#

class DataDetailsScreen(Screen):

    def __init__(self, **kw):
        super().__init__(**kw)

        # A check for absoulte first enter
        self.first_enter = True

        self.on_first_table = True
        self.current_keys = [0, 0]
        
        # If tables should be displayed in ascending or descending order
        self.sorted_descending = True

        # If dates should be all-time or only today
        self.date_range_all_time = True


    def on_pre_enter(self):
        # Checks if its the first time entering
        if self.first_enter:
            self.display_data(10)
            self.display_data(11)
            self.first_enter = False

        # Checks if theme is light or darkmode
        if current_theme_light:
            color = (0,0,0)
        else:
            color = (1,1,1)

        for i in range(10,51):
            id = encodings[i][0].lower() + encodings[i][1:]
            if current_theme_light:
                exec("self." + id + ".text_color =" + str(color))
                exec("self." + id + ".line_color =" + str(color))

        # Edge cases for extra button
        # OBS. ADD NEW BUTTONS HERE FOR LIGHT/DARK COMPATABILITY
        self.allDataBTN.text_color = color
        self.dateRangeBTN.text_color = color
        self.sortBTN.text_color = color

        # Updating tables since color will be weird otherwise
        #self.update_tables() ->>> FIX THIS!





    def display_data(self, key):
        if self.on_first_table:
            self.add_table({"center_x": 0.45, "center_y": 0.5}, key)
            self.current_keys[0] = key
            self.on_first_table = False

        else:
            self.add_table({"center_x": 0.8, "center_y": 0.5}, key)
            self.current_keys[1] = key
            self.on_first_table = True
            


    def add_table(self, position, key):
        try:
            if self.sorted_descending == True:
                if self.date_range_all_time:
                    table_data = pd.Index(DB.execCommmand("SELECT * FROM " + encodings[key] + " ORDER BY ID DESC")).values
                else:
                    table_data = pd.Index(DB.execCommmand("SELECT * FROM " + encodings[key] + " WHERE TimeData >= date('now', 'localtime') ORDER BY ID DESC")).values
            else:
                if self.date_range_all_time:
                    table_data = pd.Index(DB.execCommmand("SELECT * FROM " + encodings[key] + " ORDER BY ID ASC")).values
                else:
                    table_data = pd.Index(DB.execCommmand("SELECT * FROM " + encodings[key] + " WHERE TimeData >= date('now', 'localtime') ORDER BY ID ASC")).values
        except Exception as e:
            table_data = ()

        table = MDDataTable(
            size_hint=(0.3, 0.6),
            pos_hint=position,
            use_pagination=True,
            rows_num=10,
            column_data=[
                ("ID", dp(20)),
                ("Time", dp(20)),
                (encodings[key], dp(60))
            ],
            row_data=table_data
        )
        
        self.add_widget(table)
        self.dismiss_latest_table()


    def dismiss_latest_table(self):
        if len(self.children) > 3:
            self.remove_widget(self.children[2])

    def update_tables(self):
        self.remove_widget(self.children[1])
        self.remove_widget(self.children[0])
        
        self.add_table({"center_x": 0.45, "center_y": 0.5}, self.current_keys[0])
        self.add_table({"center_x": 0.8, "center_y": 0.5}, self.current_keys[1])


    def switch_sort(self):
        if self.sorted_descending:
            self.sorted_descending = False
            self.sortedBTN.text = "Sorted: ascending"
            self.sortedBTN.icon = "arrow-up"
        else:
            self.sorted_descending = True
            self.sortedBTN.text = "Sorted: decending"
            self.sortedBTN.icon = "arrow-down"
        
        self.update_tables()


    def switch_date_range(self):
        if self.date_range_all_time:
            self.date_range_all_time = False
            self.dateBTN.text = "Date range: today"
            self.dateBTN.icon = "tally-mark-1"
        else:
            self.date_range_all_time = True
            self.dateBTN.text = "Date range: all-time"
            self.dateBTN.icon = "all-inclusive"

        self.update_tables()

        

class WindowManager(ScreenManager):
    pass

class LoRaGUI(MDApp):

    def build(self):
        self.theme_cls.theme_style_switch_animation = True
        self.theme_cls.theme_style = "Dark"
        
        kv = Builder.load_file(os.path.realpath("my.kv"))
        Window.size = (1200, 600)
        return kv

    def on_start(self):
        Clock.schedule_interval(lambda check_theme: self.change_theme(), 0.5)

    # Switches themes of GUI light <-> dark
    def change_theme(self):
        if current_theme_light:
            self.theme_cls.theme_style = "Light"
        else:
            self.theme_cls.theme_style = "Dark"

if __name__ == "__main__":
    LoRaGUI().run()
