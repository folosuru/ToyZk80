tk80のアセンブラとかだよ。たのしいね。

## アセンブラ
Z80のアセンブラ/逆アセンブラとなっております.
`;`以降はコメントになります。
```
ToyTk80_asm [入力ファイル] -o [出力ファイル]
```
出力が指定されていないときは標準出力に出る。

入力に標準入力は使えないので、`/dev/stdin`などは入れないでください。（出力には使えます）

### アセンブル
入力ファイルのアセンブリを16進数で出力ファイルに出力する。

<details>
<summary>例</summary>

`in.asm`がこうなっているときに
```
LD A, 0
LD B, 10
loop:
ADD A,B
DEC B
LD C, A
LD A, 0
CP B
JR NZ, loop
LD A, C
HALT
```
このコマンドを実行することで
```sh
ToyTk80_asm ./in.asm -o ./out.hex
```

`out.hex`にこう出力される
```
3e00060a80054f3e00b820f87976
```

</details>

### 逆アセンブル
入力ファイルの内容を16進数で読み取り、アセンブリを出力ファイルに出力する。`--disassemble`フラグが必要。
<details>
<summary>例</summary>

`in.hex`がこうなっているとき
```
3e00060a80054f3e00b820f87976
```

このコマンドを実行することで
```sh
ToyTk80_asm ./in.hex -o ./out.asm --disassemble
```
こう出力される。
```asm
    LD A,00H
    LD B,0aH
LABEL_0: 
    ADD A,B
    DEC B
    LD C,A
    LD A,00H
    CP B
    JR NZ,LABEL_0
    LD A,C
    HALT 
```
</details>

### オプション

#### `-o [filename]`
出力先のファイルを指定する。なにも指定しない場合stdoutになる。

#### `--assemble`
アセンブルする。`--disassemble`が無い場合は自動的にこれになる。

#### `--disassemble`
逆アセンブルする。

#### `--label`
逆アセンブルの時、アセンブリの範囲内の即値をアドレスを示すものとして解釈し、全てにラベルをつける。
<details>
<summary>例</summary>

```asm
jump_addr:
JP jump_addr
```
というコードがあった時、`--label`フラグがないとこれをアセンブル→逆アセンブルした際には数値がそのまま表示される。
```asm
JP 8000H
```
`--label`フラグがある場合、アセンブル→逆アセンブルすると自動的にラベルが作成される。
```asm
LABEL_0: 
JP LABEL_0
```

ただし、それが実際アドレスであるかの検証はしないので、うまくいかないかもしれない。
</details>

#### `-start [hex]`
プログラムの開始位置を16進数で指定する。何も指定しない場合`-start 8000`と同様の動作。

#### `--address`
逆アセンブル時に、行頭にアドレスを表示する。表示用。
<details>
<summary>表示例</summary>

```sh
$ ToyTk80_asm.exe --disassemble .\in.bin --address
0x8000 |     LD A,00H
0x8002 |     LD B,0aH
0x8004 | LABEL_0: 
0x8004 |     ADD A,B
0x8005 |     DEC B
0x8006 |     LD C,A
0x8007 |     LD A,00H
0x8009 |     CP B
0x800a |     JR NZ,LABEL_0
0x800c |     LD A,C
0x800d |     HALT
```
</details>
