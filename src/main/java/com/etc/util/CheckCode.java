package com.etc.util;

public class CheckCode {
    public static void main(String[] args) {
       // String s = checkcode_0007("0501f285");
        String s = charToStr("余额不足！");
        //String s = ascii2native("余额不足！");
        System.out.println(s);
    }

    public static String checkcode_0007(String para) {
        int length = para.length() / 2;
        String[] dateArr = new String[length];

        for (int i = 0; i < length; i++) {
            dateArr[i] = para.substring(i * 2, i * 2 + 2);
        }
        String code = "00";
        for (int i = 0; i < dateArr.length; i++) {
            code = xor(code, dateArr[i]);
        }
        if (code.length() == 1) {
            code = "0" + code;
        }
        return code;
    }

    private static String xor(String strHex_X, String strHex_Y) {
        //将x、y转成二进制形式
        String anotherBinary = Integer.toBinaryString(Integer.valueOf(strHex_X, 16));
        String thisBinary = Integer.toBinaryString(Integer.valueOf(strHex_Y, 16));
        String result = "";
        //判断是否为8位二进制，否则左补零
        if (anotherBinary.length() != 8) {
            for (int i = anotherBinary.length(); i < 8; i++) {
                anotherBinary = "0" + anotherBinary;
            }
        }
        if (thisBinary.length() != 8) {
            for (int i = thisBinary.length(); i < 8; i++) {
                thisBinary = "0" + thisBinary;
            }
        }
        //异或运算
        for (int i = 0; i < anotherBinary.length(); i++) {
            //如果相同位置数相同，则补0，否则补1
            if (thisBinary.charAt(i) == anotherBinary.charAt(i))
                result += "0";
            else {
                result += "1";
            }
        }
        return Integer.toHexString(Integer.parseInt(result, 2));
    }

    //string类型的转为十六进制的字符串
    public static String charToStr(String str){
        try{
            byte[] chars = str.getBytes("GBK");
            int result;
            StringBuffer string = new StringBuffer();

            for (int i = 0; i < chars.length; i++) {
                result = chars[i];
                string.append(Integer.toHexString(result).replaceAll("ffffff", ""));

            }
            System.out.println(string + "十六进制的字符串！！");
            return string.toString();
        }catch (Exception e){
            e.printStackTrace();
        }
      return null;
    }
}
