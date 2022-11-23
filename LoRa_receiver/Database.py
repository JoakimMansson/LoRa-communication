import sqlite3
import pandas as pd


class sqlConnection():

    def __init__(self) -> None:
        self.conn = sqlite3.connect("data")
        self.cursor = self.conn.cursor()


    # Declare name, datatype and constraints
    # EX:
    # CREATE TABLE tableName(
    # id INT PRIMARY KEY,
    # name VARCHAR(20)
    # );
    # ex: createTable("tableName", "id INTEGER PRIMARY KEY AUTOINCREMENT", "name VARCHAR(20)")
    def createTable(self, tableName: str, *argv: str) -> None:
        command = "CREATE TABLE " + tableName + "("
        
        allArgs = []
        for arg in argv:
            allArgs.append(arg)

        for i in range(len(allArgs)):
            if i == len(allArgs) - 1:
                command = command + allArgs[i] + ");"
                break
            
            command = command + " " + allArgs[i] + ","

        try:
            self.cursor.execute(command)
        except Exception as e:
            print("Failed creating table: " + tableName + "\n" + str(e))


    def insertInto(self, tableName: str, columns: str, values: str) -> None:
        command = "INSERT INTO " +  tableName + " " + columns + " "
        command = command + "VALUES " + values + ";"
        self.cursor.execute(command)
        self.conn.commit()
        #print(command)


    def selectAll(self, tableName: str) -> None:
        command = "SELECT * FROM " + tableName
        self.cursor.execute(command)
        
        # Pretty print
        #print(pd.read_sql_query(command, self.conn))

        print(self.cursor.fetchall())
    
    
    def execCommmand(self, cmd: str):
        self.cursor.execute(cmd)

        obj = self.cursor.fetchall()
        if len(obj) == 0:
            self.conn.commit()
        else:
            return obj
    
    def getCursor(self) -> object:
        return self.cursor




if __name__ == "__main__":
    sql = sqlConnection()
    #sql.createTable("BMS", "bms_ID INTEGER PRIMARY KEY AUTOINCREMENT", "packCurrent VARCHAR(20)")
    #sql.insertInto("BMS", "", "(1 ,'1337')")
    #sql.selectAll("BMS")
    #print(sql.execCommmand("SELECT * FROM BMS"))
