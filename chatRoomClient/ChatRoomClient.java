import java.net.Socket;
import java.net.SocketException;
import java.io.*;
import java.lang.Integer;
import java.lang.Thread;

public class ChatRoomClient{
	public static void main(String[] args) throws Exception{
		final int bufSize = 1024;
		String host = args[0];
		int port = Integer.parseInt(args[1]);
		Socket socket = new Socket(host, port);
		System.out.println("Established socket and connected to server: "+ host);
		System.out.println("Please start chatting :D");
		Thread reader_thread = new Reader(socket);
		reader_thread.start();
		BufferedReader tx = new BufferedReader(new InputStreamReader(System.in));
		BufferedWriter out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
		while(true){
			String text = tx.readLine();
			try{
				out.write(text, 0, text.length());
				out.flush();
				}catch(SocketException s){
				System.out.println("Server may be shutted down, write thread exit");
				break;
				}
			if(text.equals("quit"))
				break;
		}
		try{
			socket.close();
		}catch(Exception s){}

		reader_thread.join();
		System.out.println("Client exit");
	}

	public static class Reader extends Thread{
		private Socket socket;

		public Reader(Socket arg){
			this.socket = arg;
		}

		@Override
		public void run(){
			try{
				BufferedReader in = new BufferedReader(new InputStreamReader
									(this.socket.getInputStream()));
				while(true){
						String text;
						if((text = in.readLine())== null){
							System.out.println("Server may be shutted down, read thread exit");
							break;
						}
						System.out.println(text);
				}
			}catch(IOException e){
			}
			try{
				this.socket.close();
			}catch(Exception s){}
			System.out.println("socket close, type 'quit' to exit");
		}
	}
}