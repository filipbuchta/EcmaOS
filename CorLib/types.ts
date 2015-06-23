
class int32 {
    _value: int32;


    static toString(value: int32): string {

        let stringSize: int32 = 0;
        //let negative = false;
        //if (value < 0) {
        //    negative = true;
        //}
        let temp = value;

        stringSize = 1;
        if (temp <= 9) stringSize = 1;
        else if (temp <= 99) stringSize = 2;
        else if (temp <= 999) stringSize = 3;
        else if (temp <= 9999) stringSize = 4;
        else if (temp <= 99999) stringSize = 5;
        else if (temp <= 999999) stringSize = 6;
        else if (temp <= 9999999) stringSize = 7;
        else if (temp <= 99999999) stringSize = 8;
        else if (temp <= 999999999) stringSize = 9;
//        else if Integer.MAX_VALUE

        
        let chars = new char[stringSize];
        if (temp == 0) {
            chars[0] = '0';
        }
        else {
            let i: int32 = stringSize - 1;
            while (temp > 0) {
                let digit = temp % 10;
                if (digit == 0) chars[i] = '0';
                else if (digit == 1) chars[i] = '1';
                else if (digit == 2) chars[i] = '2';
                else if (digit == 3) chars[i] = '3';
                else if (digit == 4) chars[i] = '4';
                else if (digit == 5) chars[i] = '5';
                else if (digit == 6) chars[i] = '6';
                else if (digit == 7) chars[i] = '7';
                else if (digit == 8) chars[i] = '8';
                else if (digit == 9) chars[i] = '9';
                temp = temp / 10;
                i--;
            }
        }
        return new string(chars);
    }
}

class void {

}

class boolean {
    _value: boolean;

}

class char {
    _value: char;
}


class string {
    _chars: char[];
    //getLength(): int32 { return this.chars.length; }

    constructor() {
    }

    constructor(chars: char[]) {
        _chars = chars;
    }
}



class Console {
    //static declare log(value: int32): void;
    static declare log(value: string): void;

    static log(value: int32): void {
        Console.log(int32.toString(value));
    }
}