import sys
import crypt
from hmac import compare_digest as compare_hash

def checkPass (passw, key, inhash):
    hashP = crypt.crypt(passw, key)
    if inhash == hashP:
        print(passw)
        sys.exit()

def main():
    if len(sys.argv) != 2:
        print("Usage ./crack <hash>")
        return 1
        
    hashString = sys.argv[1]
    password = ""
    salt = '50'
    symb = [0] * 52
    upper = 65
    lower = 97
    for i in range(52):
        symb[i] = chr(lower + i)
        if i > 25:
            symb[i] = chr(upper + i - 26)
        
    for my_var1 in range (52):
        password = symb[my_var1]
        checkPass (password, salt, hashString)
        
        for my_var2 in range (52):
            temp = symb[my_var2]
            password2 = password + temp
            checkPass (password2, salt, hashString)
            
            for my_var3 in range (52):
                temp2 = symb[my_var3];
                password3 = password2 + temp2
                checkPass (password3, salt, hashString);
            
                for my_var4 in range (52):
                    temp3 = symb[my_var4]
                    password4 = password3 + temp3
                    checkPass (password4, salt, hashString)

    return 0
    
if __name__ == "__main__":
    main()