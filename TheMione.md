# Mione Programming Language

一款簡易語法規則的新型程式語言。其程式碼參考了shell script的設計，每行都有他固定的「執行目的」。


## 了解語法
### 正規語法展示
```
/*
    myMione.mio 
    Created by Hxx on 2024/10/05
*/

input "io";

set x = 100;

if (x > 99) then range
    print("number of x is more then 99");
end;

return 0;
```

### 語法列車
想像有一條列車（train）正從Mione火車站出發，其中最前的車廂為頭車廂（head-carriage）是整台火車的控制者，他可以決定旅途中的岔路往哪走，而其他車廂為子車廂（body-carriages）必須隨著頭車廂。每一節車廂（carriage）都有一位他的車廂管理者（carriage-manager）與車廂乘客（carriage-passengers），車廂管理者能操控者所處的車廂。而車廂管理者與車廂乘客分別是類行為HEAD、PROMPT與SYMBOL、VARIABLE、VALUE的成員。

Mione的每一句程式句就是一條列車，每節車廂都是一個段落，每位成員都是Mione的有效語詞單元。而Mione火車站就是文件的源代碼，有多少句有效語句就有多少條列車；有多少句段落就有多少節車廂；有多少語詞單元就有多少位欲乘成員。

```
set x = 100;

set x //單獨一個Carriage，因carriage所處位置最前，所以為一個head-carriage。
//其中 `set`為carriage-manager，`x`為carriage-passengers。
= 100 //單獨一個Carriage，因carriage所處位置在後，所以為一個body-carriage。
//以上Carriages組成一個Train
```

突然某天，前頭車廂的車廂管理員睡過頭而沒來上班，導致沒有人能決定路途中的岔路要往哪，這時所有車廂的管理員會一起協商整列列車所行使的路徑。而這時列車就從一般列車（normal-train）變成簡易列車（simply-train），差別就在於頭列車是否有車廂管理員。

```
x = 100;

x //單獨一個Carriage，因carriage所處位置最前，所以為一個head-carriage。
//其中carriage-manager不存在，所以會令此Train為simply-train，`x`為carriage-passengers。
= 100 //單獨一個Carriage，因carriage所處位置在後，所以為一個body-carriage。
//其中`=`為carriage-manager，`100`為carriage-passengers。
//`=`最常出現在關於「賦予或設置」，所以該Train將會有「賦予或設置」的功能。
//以上Carriages組成一個Train
```

每位欲乘成員都需要依次上當前列車與當前車廂，但為了更好管理每條列車與車廂，Mione鐵路管理局嚴格規定了一些規則：
1. 若VARIABLE或VALUE與VARIABLE或VALUE彼此接觸（如：`100 200`），鐵路局會將他們分配到不同的列車中，這也意味著最前的VARIABLE或VALUE將是當前列車最後的成員，最後的VARIABLE或VALUE將是下條列車最前的成員（進而形成簡易列車），隨後的成員都將搭乘該列車與車廂直到觸發相對應的規則。

如以下程式碼：
```
set a = 100
a = 200
```
轉為可被記錄的語詞單元形式（實際上不包括空格）：
```
set a = 100 a = 200
```
依規則成的列車：
`set a = 100` 與 `a = 200` 。

2. 若SYMBOL不同意與後方的乘客接觸，鐵路局將會把該SYMBOL列為當前列車的最後乘客，隨後的成員都將搭乘下一條列出直到觸發相對應的規則（進而形成簡易列車）。又倘若不被同意的一方為SYMBOL，且同意與原乘客接觸，原乘客會重新同意與改乘客接觸，且不會發生任何事。（欲知每個SYMBOL所同意接觸的乘客請參考語詞類型之SYMBOL小節）

如以下程式碼：
```
set a = (200 + 300)
a = 200 + -200
```
轉為可被記錄的語詞單元形式（實際上不包括空格）：
```
set a = (200 + 300) a = 200 + -200
```
依規則成的列車：
`set a = (200 + 300)` 與 `a = 200 + -200` 。

3. 若成員為一個HEAD，鐵路局會將他分配成變成下部列車的頭車廂管理員，隨後的成員都將搭乘該列車與車廂直到觸發相對應的規則。

如以下程式碼：
```
set a = 100
set b = 200
```
轉為可被記錄的語詞單元形式（實際上不包括空格）：
```
set a = 100 set b = 200
```
依規則成的列車：
`set a = 100` 與 `set b = 200` 。

4. 若成員為一個PROMPT，鐵路局會將他分配成同列車內的下節車廂管理員，隨後的成員都將搭乘該車廂直到觸發相對應的規則。
5. 若遇到非有效語詞的`;`符號，且後方乘客不會與該符號觸發以上規則，鐵路局會阻擋他上列車，並分配隨後的成員搭乘下一列列車直到觸發相對應的規則。

如以下程式碼：
```
set a = 100; //符號有效，`(` SYMBOL不會與該符號觸發規則。
(function end)(); //符號無效，該符號會與HEAD觸發第一條規則。
set a = 200
```
轉為可被記錄的語詞單元形式（實際上不包括空格）：
```
set a = 100; (function end)(); set a = 200
```
依規則成的列車：
`set a = 100` 、`(function end)()` 與`set a = 200`。

### carriage成員/有效語詞單元在列車中的類型
#### carriage-manager
Carriage-manager，中譯車廂管理員，是段落的核心，他的能告訴Mione這段段落是否為head-carriage或body-carriage。

若為HEAD，他能宣告該段落為head-carriage，也能間接的告訴Mione所在程式句的概要目的。

若為PROMPT，他能宣告該段落為body-carriage，也能告訴Mione該段落詳細目的的方式。

在carriage裡，carriage-manager只能有一位成員，也就是單個HEAD或PROMPT。
#### carriage-passengers
Carriage-passengers，中譯車廂乘客，是告訴Mione該段落的目的為何（包括head-carriage的概要目的與body-carriages的詳細目的）。

在carriage裡，carriage-passengers能有多個成員，也就能多個SVV（SYMBOL、VARIABLE與VALUE）。
### 段落／carriage類型
#### head-carriage
Head-carriage，中譯頭車廂，是整句程式句的核心，他能告訴Mione這句程式碼的概要目的。
#### body-carriages
Body-carriage，中譯子車廂，是程式句中的附加內容，他可以詳細的告訴Mione這程式句的詳細目的。

### 程式句／train類別

#### normal-train
normal-train，中譯一般列車，是指程式句中存在head-carriage的carriage-manager。

#### simply-train
simply-train，中譯簡易列車，是指程式句中並不真實存在head-carriage的carriage-manager。

而body-carriages的carriage-manager各自以「最常出現在哪個head-carriage的carriage-manager」來做相互配合以彌補本句不存在的head-carriage的carriage-manager。




## 了解語詞
### 字元解釋與語詞單元
語詞單元（簡稱語詞），是指解釋器從源代碼提取出的特定字串，其若為有效語詞則能在經過後續的解釋獲得各自的類型。

解釋器會依源代碼的每個字元一一做解釋，並創建相關的語詞單元，其規則如下：
* 令當前字元為第`n`項，上個字元為第`n-1`項。
* 令`0`~`9`為數字字元、`"`為雙引號字元、`'`為單引號字元。
1. 若第`n`項字元類別與第`n-1`項字元類別不同，則第`n-1`項為當前語詞單元的最後可見字元，第`n`項字元為新語詞單元的第一字元。


### 語詞類型
又稱五大類別或五大別類，每個類型都在Train中扮演著重要不可或缺的角色。
語詞類型只會分配給有效語詞。
#### HEAD
HEAD，中譯標頭，可以成為head-carriage的carriage-manager而告訴Mione所在程式句的概要目的。

常見的HEAD關鍵字如下：
| 關鍵字 | 說明 |
|:-:|:-:|
|`set`|表示程式句的目的為「賦予及設定」|

程式碼：
```
set x = 100; //其中的`set`為HEAD
```
#### PROMPT
RPOMPT，中譯題詞，在normal-train中可以成為body-carriages的carriage-manager而告訴Mione所在程式句的詳細目的。在simply-train中可以與其他body-carriages的carriage-manager協調而成為偽head-carriages的carriage-manager。

常見的PROMPT關鍵字如下：
| 關鍵字 | 說明 |
|:-:|:-:|
|`=`|詳細描述程式句的目的為「賦予及設定值給相對應的head-carriage的carriage-passengers」|

程式碼：
```
set x = 100; //其中的`=`為PROMPT
```
#### SYMBOL
SYMBOL，中譯符號，可以將指定的carriage-passengers混合成新的carriage-passengers。


*以下用`n`表示SYMBOL成員位置，`n+1`為SYMBOL成員的右一位成員。如：`1+2`，則`n+1`表示`2`;`n-1`表示`1`。*

常見的SYMBOL關鍵字如下：

| 關鍵字 | 說明 |
|:-:|:-:|
|`+`|將`n+1`成員與`n-1`成員相加|

程式碼：
```
return 10+20; //其中的`+`為SYMBOL
```

#### VARIABLE
VARIABLE，中譯變數，是一種被動的carriage-passengers成員，能使用所攜帶的VARIABLE LINK來進行運算。

若有效的語詞單元未能配對成HEAD、PROMPT、SYMBOL與VALUE，將會自動被分配為VARIABLE。

##### VARIABLE LINK
VARIABLE LINK，中譯變數連結。

其可以指向另個VARIABLE LINK，或是直接指向VARIABLE核心(即變數本身)。

在同個作用域內，相同屬性的VARIABLE(如：名稱、編號等...)會被分配相同的VARIABLE LINK。
若屬性相同但在更低層的作用層，則會創建新的VARIABLE LINK並指向同屬性的上級VARIABLE LINK。

任何VARIABLE LINK都會在運算前「尋路」找到所指向的VARIABLE核心再進行運算。

#### VALUE

## 作用層與語式
### 作用層
作用層，指包裹一句或多句程式句的載體。

其可分為兩種類型：主作用層與子作用層。

#### 主作用層
主作用層，是指未被任何作用層包裹的作用層。

主作用層是不需任何語詞幫助包裹的。

例如以下程式碼：
```
//
set x = 100; 
// 未被包裹，所以為主作用層。
```

#### 子作用層
子作用層，是指已被作用層包裹的作用層。

子作用層會需要語詞幫助包裹。如：`range`、`function`、`end`。

例如以下程式碼：
```
if true then 
range //
    set x = 200; //被range ... end包裹，為子作用層。
end //range ... end為子作用層本身
```


### 語式
語式，指作用層所包裹的程式句運行邏輯的規範。

#### 主語式
主語式，是作用在主作用層的語式。
#### 函數語式
#### 執行塊語式
#### 表單語式




## 容器與資源

容器與資源是表示carriage-passengers在經過運算後是被寫入或是被讀出的類型。其次，每個carriage-manager都會指定所在carriage的carriage-passengers運算後為何者類型，若所指定類型不匹配運算後的類型將會報錯。

如：`set x = 100;`中，`x`與`100`都是各自carriage的carriage-passengers，而`x`為容器，`100`為資源。

### 容器
容器是表示運算過後的carriage-passengers為寫入的類型。

若carriage-manager指定的carriage-passengers類型為容器，在運算過後所得的carriage-passengers應為VARIABLE，否則將會報錯。

### 資源
資源是表示運算過後的carriage-passengers為讀取的類型。

若carriage-manager指定的carriage-passengers類型為資源，在運算過後所得的carriage-passengers應為VALUE，否則將會報錯。