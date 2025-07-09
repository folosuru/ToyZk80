# Assembler
## オプションなど
### `-o [filename]`
出力先のファイルを指定する。なにも指定しない場合stdoutになる。

### `--assemble`
アセンブルする。`--disassemble`が無い場合は自動的にこれになる。

### `--disassemble`
逆アセンブルする。

### `--label`
逆アセンブルの時、アセンブリの範囲内の即値をアドレスを示すものとして解釈し、全てにラベルをつける。
<details>
<summary>例</summary>

```asm
jump_addr:
JP jump_addr
```
というコードがあった時、`--label`フラグがないとこれをアセンブル→逆アセンブルした際にはメモリアドレスの数値がそのまま表示される。
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

### `-start [hex]`
プログラムの開始位置を16進数で指定する。何も指定しない場合`-start 8000`と同様の動作。

### `--address`
逆アセンブル時に、行頭にアドレスを表示する。表示用。
<details>
<summary>表示例</summary>

```sh
$ ToyZk80_asm.exe --disassemble .\in.bin --address
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


