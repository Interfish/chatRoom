import java.net.Socket;
import java.io.*;

public class Client {
  public static void main(String[] args) throws Exception{
    final int bufSize = 1024;
    String host = "192.168.0.3";
    int port = 1234;
    Socket socket = new Socket(host,port);
    System.out.println("connected to server: "+ host);
    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    System.out.println("Request:");
    String str = br.readLine();
    DataOutputStream out = new DataOutputStream(socket.getOutputStream());
    out.writeUTF(str);
    out.flush();
    DataInputStream in = new DataInputStream(socket.getInputStream());
    String response = in.readUTF();
    System.out.println("server says: " + str);
    socket.close();
  }
}