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
  
## 4. car : 取左邊的值, cdr : 取右邊的值  
![image](https://github.com/qazzy5566/MyProgram/blob/99c51a98c8a3b9f3af41c20270a23d3a1de75dba/OurScheme/img/5.car_cdr.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/99c51a98c8a3b9f3af41c20270a23d3a1de75dba/OurScheme/img/5.mix.png)  
    
## 5. 檢查型別的Function, #t = True, nil = False  
![image](https://github.com/qazzy5566/MyProgram/blob/99c51a98c8a3b9f3af41c20270a23d3a1de75dba/OurScheme/img/6.primitive_predicate.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/99c51a98c8a3b9f3af41c20270a23d3a1de75dba/OurScheme/img/6.1.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/99c51a98c8a3b9f3af41c20270a23d3a1de75dba/OurScheme/img/6.2.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/99c51a98c8a3b9f3af41c20270a23d3a1de75dba/OurScheme/img/6.3.png)  
  
## 6. 算術運算  
![image](https://github.com/qazzy5566/MyProgram/blob/c73af81f430482ff213e890477f288e31b2b4d22/OurScheme/img/7.arithmetic.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/c73af81f430482ff213e890477f288e31b2b4d22/OurScheme/img/7.arithmetic2.png)  
  
## 7. 邏輯運算  
![image](https://github.com/qazzy5566/MyProgram/blob/c73af81f430482ff213e890477f288e31b2b4d22/OurScheme/img/7.logical.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/c73af81f430482ff213e890477f288e31b2b4d22/OurScheme/img/8.and_or.png)  
  
## 8. String操作  
![image](https://github.com/qazzy5566/MyProgram/blob/c73af81f430482ff213e890477f288e31b2b4d22/OurScheme/img/7.string.png)  
  
## 9.  eqv? 和 equal?  
eqv? returns "true" only when the two being compared objects are atoms (except in the case of strings)  
or when the two being compared objects "occupy the same memory space".  
equal?, on the other hand, is the usual notion of equality comparison.  
![image](https://github.com/qazzy5566/MyProgram/blob/796e718bf6f79256e7fb77b0f47dc94473472143/OurScheme/img/8.eq.png)  
  
## 10. condition 條件式  
![image](https://github.com/qazzy5566/MyProgram/blob/796e718bf6f79256e7fb77b0f47dc94473472143/OurScheme/img/9.condition.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/796e718bf6f79256e7fb77b0f47dc94473472143/OurScheme/img/9.condition2.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/796e718bf6f79256e7fb77b0f47dc94473472143/OurScheme/img/9.condition3.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/796e718bf6f79256e7fb77b0f47dc94473472143/OurScheme/img/9.condition4.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/796e718bf6f79256e7fb77b0f47dc94473472143/OurScheme/img/9.condition5.png)  
  
## 11. 一些指令組合  
begin : 去最右邊的值  
![image](https://github.com/qazzy5566/MyProgram/blob/8c7bf24cc5504e487fd2aa01b8629cb218f19bef/OurScheme/img/10.composition.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/8c7bf24cc5504e487fd2aa01b8629cb218f19bef/OurScheme/img/10.composition2.png)  
  
## 12. clean environment  
![image](https://github.com/qazzy5566/MyProgram/blob/8c7bf24cc5504e487fd2aa01b8629cb218f19bef/OurScheme/img/11.clean.png)  
  
## 13. define變數為內建函數  
![image](https://github.com/qazzy5566/MyProgram/blob/8c7bf24cc5504e487fd2aa01b8629cb218f19bef/OurScheme/img/12.other_define.png)  
  
## 14. let : 定義一些變數(locally)，然後執行
![image](https://github.com/qazzy5566/MyProgram/blob/8c7bf24cc5504e487fd2aa01b8629cb218f19bef/OurScheme/img/13.let.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/8c7bf24cc5504e487fd2aa01b8629cb218f19bef/OurScheme/img/13.let2.png)  
  
## 15. lambda  
![image](https://github.com/qazzy5566/MyProgram/blob/8c7bf24cc5504e487fd2aa01b8629cb218f19bef/OurScheme/img/14.lambda.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/8c7bf24cc5504e487fd2aa01b8629cb218f19bef/OurScheme/img/14.lambda2.png)  
![image](https://github.com/qazzy5566/MyProgram/blob/8c7bf24cc5504e487fd2aa01b8629cb218f19bef/OurScheme/img/14.define_lambda.png)  
  
## 16. 結束程式 (exit)  
![image](https://github.com/qazzy5566/MyProgram/blob/8c7bf24cc5504e487fd2aa01b8629cb218f19bef/OurScheme/img/END.png)  
