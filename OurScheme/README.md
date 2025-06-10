大學時期寫的直譯器(Interpreter)，可以輸入一些指令然後產生執行結果  
(程式一開始要輸入一個數字，是當初作業的測試數據號碼)

# 指令說明  

## 1.cons 和 list : Constructors  
![image](https://github.com/qazzy5566/MyProgram/blob/f5cb38a4c86b15e8b2f78bc33189ce6a8bd0e090/OurScheme/img/1.cons.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/829c767e885db395a2db8641ce9fda71332c28a8/OurScheme/img/3.list.png)  
  
## 2.define : 定義物件  
![image](https://github.com/qazzy5566/MyProgram/blob/829c767e885db395a2db8641ce9fda71332c28a8/OurScheme/img/2.define.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/829c767e885db395a2db8641ce9fda71332c28a8/OurScheme/img/2.define_error.png)  
  
## 3.結合 1. 和 2. 的指令  
![image](https://github.com/qazzy5566/MyProgram/blob/829c767e885db395a2db8641ce9fda71332c28a8/OurScheme/img/combine123.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/829c767e885db395a2db8641ce9fda71332c28a8/OurScheme/img/combine123_2.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/829c767e885db395a2db8641ce9fda71332c28a8/OurScheme/img/combine123_3.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/99c51a98c8a3b9f3af41c20270a23d3a1de75dba/OurScheme/img/4.func.png)  
  
## 4. car : 取左邊的值  
##    cdr : 取右邊的值  
![image](https://github.com/qazzy5566/MyProgram/blob/99c51a98c8a3b9f3af41c20270a23d3a1de75dba/OurScheme/img/5.car_cdr.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/99c51a98c8a3b9f3af41c20270a23d3a1de75dba/OurScheme/img/5.mix.png)  
    
## 5. 檢查型別的Function, #t = True, nil = False  
![image](https://github.com/qazzy5566/MyProgram/blob/99c51a98c8a3b9f3af41c20270a23d3a1de75dba/OurScheme/img/6.primitive_predicate.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/99c51a98c8a3b9f3af41c20270a23d3a1de75dba/OurScheme/img/6.1.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/99c51a98c8a3b9f3af41c20270a23d3a1de75dba/OurScheme/img/6.2.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/99c51a98c8a3b9f3af41c20270a23d3a1de75dba/OurScheme/img/6.3.png)  
  
