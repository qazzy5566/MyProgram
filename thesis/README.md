我的碩士論文程式碼

訓練資料: https://www.kaggle.com/datasets/hasyimabdillah/workoutfitness-video  
論文全文: CVGIP2024-fullpaper-word_template 楊侑霖 v.1.2.pdf  

論文摘要:

日常運動或運動對於確保人類生活的健康非常重要。由於人工智慧 (AI) 技
術的進步，本研究的目標是開發一種使用 MediaPipe 和長短期記憶 (LSTM) 網
路自動識別人類運動動作的系統，利用收集的影片資料集用於訓練和測試我們的
系統模型。我們的實驗研究表明，我們的系統可以達到 90% 以上的準確率、精
確度和召回率，這表明在資料集中具有相對良好的性能。總而言之，我們的系統
模型可以產生潛在的解決方案來檢測和分類人類的鍛鍊行為，從而發展成人工智
慧教練、多媒體、人機互動或其他應用。

ABSTRACT:

Daily workout or exercise is important to ensure good health in human’s life. Because of advances of artificial 
intelligence (AI) techniques, the objective of this study is to develop a system for automatic recognition of 
human work/exercises using MediaPipe and Long-Short Term Memory (LSTM) networks. A video dataset was 
previously collected and used to train and test our system model. Our experimental study demonstrated that 
our system could achieve the accuracy, precision, and recall of over 90%, which indicates relatively good 
performance in the dataset. In summary, our system model could yield potential solutions to detect and 
classify human’s workout/exercise behavior, thus leading to AI coach, multimedia, human-computer 
interaction, or other applications. 

檔案說明:  
  
mkdir.py : 自動建22個資料夾，用來放處理過的資料  
  
SetVideoData.py : 把原始資料集用MediaPipe偵測產生Landmark資料，用Landmark資料來訓練  
  
train2.py : 主要訓練模型的程式，包含訓練測試及儲存模型、產生Loss、Accuracy、Precision、Recall、Confusion_Matrix  
  
confusion_matrix : 同train.py裡的Confusion_Matrix  
  
grid_test.py : 產生一些偵測結果的圖片  
  
image_grid.py : 產生一些原始訓練資料的圖片  
  
Final_test : 最後測試模型，輸入影片從MediaPipe生成Landmark到模型預測，然後輸出結果  
