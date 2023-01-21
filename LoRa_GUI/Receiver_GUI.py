import os

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




class MainScreen(Screen):
    dialog = None


    def __init__(self, **kw):
        super().__init__(**kw)
        self.USB_connected = "False"
        self.connection_established = "False"

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



class DataDetailsScreen(Screen):

    def __init__(self, **kw):
        super().__init__(**kw)

        self.encodings = {
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
            24: "InputPoweruSupplyFailed",
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
            47: "BusCurrent",
            48: "Velocity",
            49: "MotorCurrent",
            50: "OutputVoltagePWM"
        }

    def display_data(self, btn_index):
        print(self.encodings[btn_index])

        table = MDDataTable(
            size_hint=(0.3, 0.6),
            pos_hint={"center_x": 0.4, "center_y": 0.5},
            use_pagination=True,
            column_data=[
                ("Time", dp(30)),
                (self.encodings[btn_index], dp(60))
            ]
        )
        self.add_widget(table)

    



class WindowManager(ScreenManager):
    pass

class AlarmApp(MDApp):

    def build(self):
        self.theme_cls.theme_style = "Dark"
        kv = Builder.load_file(os.path.realpath("my.kv"))
        Window.size = (1920, 1080)
        return kv





if __name__ == "__main__":
    AlarmApp().run()
