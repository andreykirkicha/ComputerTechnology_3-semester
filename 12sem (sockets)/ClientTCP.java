// Отладить работу совместно с сервером
// Добавить в клиент и сервер команду завершения сервера.
package clienttcp;

import java.io.*;
import java.net.*;

class ClientTCP extends Thread
{
  public static void main(String args[])
  {
    try
    {
      // открываем сокет и коннектимся к localhost:3128
      // получаем сокет сервера
      Socket s = new Socket("localhost", 3128);

      // берём поток вывода и выводим туда первый аргумент
      // заданный при вызове, адрес открытого сокета и его порт
      args[0] = args[0]+"\n"+s.getInetAddress().getHostAddress()
                                      +":"+s.getLocalPort();
      // Печатаем запрос
      System.out.println(args[0]);
      s.getOutputStream().write(args[0].getBytes());

      // читаем ответ
      byte buf[] = new byte[64*1024];
      int r = s.getInputStream().read(buf);
      String data = new String(buf, 64*2, r);

      // выводим ответ в консоль
      System.out.println(data);
    }
    catch(Exception e)
    {
      // вывод исключений
      System.out.println("init error: "+ e);
    } 
}
}