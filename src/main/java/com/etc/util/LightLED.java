package com.etc.util;

import java.io.*;
import java.net.Socket;

public class LightLED {
    private Socket client;
    private String host = "192.168.27.188";
    private int port = 2001;
    private CheckCode testMain = new CheckCode();

    public LightLED() throws IOException {

        System.out.printf("connected led %s:%d...\n", host, port);
        client = new Socket(host, port);
        System.out.println("LED客户端启动成功\n");
    }

    /**
     * 语音显字同时进行
     *
     * @throws Exception
     */
    public void doLED() throws Exception {

    }

    private PrintWriter getWriter(Socket s) throws IOException {

        OutputStream ous = s.getOutputStream();
        return new PrintWriter(ous, true);
    }

    private BufferedReader getReader(Socket s) throws IOException {

        InputStream ins = s.getInputStream();
        return new BufferedReader(new InputStreamReader(ins));
    }

    /**
     * 将字符串形式表示的十六进制数转换为byte数组
     */
    public byte[] hexStringToBytes(String hexString) throws Exception {

        String[] start;
        int m = hexString.length() / 2;
        if (m * 2 < hexString.length()) {
            m++;
        }
        start = new String[m];
        int j = 0;
        for (int i = 0; i < hexString.length(); i++) {
            if (i % 2 == 0) {//每隔两个
                start[j] = "" + hexString.charAt(i);
            } else {
                start[j] = start[j] + hexString.charAt(i);//将字符加上空格
                j++;
            }
        }

        byte[] bytes = new byte[start.length];

        for (int i = 0; i < start.length; i++) {
            char[] hexChars = start[i].toCharArray();
            bytes[i] = (byte) (charToByte(hexChars[0]) << 4 | charToByte(hexChars[1]));
        }

        return bytes;
    }

    //byte字节数组转成十六进制字符串
    public String BinaryToHexString(byte[] bytes) {
        String hexStr = "0123456789ABCDEF";
        String result = "";
        String hex = "";
        for (byte b : bytes) {
            hex = String.valueOf(hexStr.charAt((b & 0xF0) >> 4));
            hex += String.valueOf(hexStr.charAt(b & 0x0F));
            result += hex + "";
        }
        return result;
    }

    public String printHexBinary(String hexStr) {
        return null;
    }

    private static byte charToByte(char c) {
        return (byte) "0123456789abcdef".indexOf(c);

    }

    /**
     * 语音播报
     */

    public String getVoice(String prefix, String start, String destId, String cmd, String data) throws Exception {
        String len1 = null;//外面长度
        String len2 = null;//语音包长度
      //  String voiceCommand = null;
        String xrl = null;//校验
       // String end = null;
        OutputStream pw = client.getOutputStream();
        len2 = Integer.toHexString((data.length() + 8) / 2);
        if (len2.length() == 1) {
            len2 = "0" + len2;
        }
        len1 = Integer.toHexString((data.length() + 16) / 2);
        if (len1.length() == 1) {
            len1 = "0" + len1;
        }

        xrl = testMain.checkcode_0007(len2 + destId + cmd + data);
       // voiceCommand = start + len2 + destId + cmd + data + xrl;
        StringBuffer endStr = new StringBuffer();
        endStr.append(prefix);
        endStr.append(len1);
        endStr.append(start);
        endStr.append(len2);
        endStr.append(destId);
        endStr.append(cmd);
        endStr.append(data);
        endStr.append(xrl);
        //end = prefix + len1 + voiceCommand;
        System.out.println(endStr + "语音最终字符");
        byte[] tmp = hexStringToBytes(endStr.toString());
        //以byte数组发送
        for (int i = 0; i < tmp.length; i++) {
            pw.write(tmp[i]);
        }
        pw.flush();//立刻发送数据
        String line = null;
        BufferedReader br = new BufferedReader(new InputStreamReader(client.getInputStream()));
        System.out.println("语音完毕！！！");
        return null;
    }

    /**
     * 显示内容
     */
    public String getDisplay(String condition) throws Exception {
        String prefix = "faaf";
        String style = "00";
        String on = testMain.charToStr(condition);
        OutputStream pw = client.getOutputStream();
        //长度
        String leng = Integer.toHexString((prefix.length() + on.length() + 6) / 2);
        if (leng.length() == 1) {
            leng = "0" + leng;
        }

        //校验的字符串
        String checkR = testMain.checkcode_0007(leng + on);
        //String endStr = prefix + leng + style + on + checkR;
        StringBuffer endStr = new StringBuffer();
        endStr.append(prefix);
        endStr.append(leng);
        endStr.append(style);
        endStr.append(on);
        endStr.append(checkR);
        System.out.println(endStr + "内容最终字符");
        //16进制字符串转换成byte数组
        byte[] tmp = hexStringToBytes(endStr.toString());
        //以byte数组发送
        for (int i = 0; i < tmp.length; i++) {
            pw.write(tmp[i]);
        }
        pw.flush();//立刻发送数据
        String line = null;
        BufferedReader br = new BufferedReader(new InputStreamReader(client.getInputStream()));
        System.out.println("内容完毕！！！");
        return null;
    }

}
