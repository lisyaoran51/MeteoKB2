using UnityEngine;
using System.Collections;
using System.Collections.Generic;

using TechTweaking.Bluetooth;
using UnityEngine.UI;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;
using System.Linq;
using System;
using System.Text;

public class AutoConnect : MonoBehaviour
{

	private  BluetoothDevice device;
	public Text statusText;

	void Awake ()
	{
		device = new BluetoothDevice ();

        TextAsset textAsset = Resources.Load<TextAsset>("aaa");

        if (textAsset == null)
        {
            Debug.Log("Resource load failed.");
        }

        byte[] bytes = textAsset.bytes;

        Debug.Log(BitConverter.ToString(bytes));


        for(int i = 0; i < 0; i++)
        {
            ushort segmentSize = (ushort)(bytes.Length - i*512 > 512 ? 512 : bytes.Length - i * 512);

            uint command = 0x000803;
            ushort length = (ushort)(sizeof(uint) + sizeof(ushort) + sizeof(byte) * 16 + sizeof(ushort) * 2 + sizeof(byte) * segmentSize);
            string fileName = "aaaaaaaaaaaaa.sm";
            ushort order = (ushort)i;
            ushort amount = (ushort)(bytes.Length / 512 + (bytes.Length % 512 > 0 ? 1 : 0));
            byte[] fileSegment = new byte[segmentSize];
            Array.Copy(bytes, i * 512, fileSegment, 0, segmentSize);

            byte[] packetInByteArray = new byte[length];

            byte[] commandInByte = BitConverter.GetBytes(command);
            byte[] lengthInByte = BitConverter.GetBytes(length);
            char[] fileNameInChar = fileName.ToCharArray();
            byte[] fileNameInByte = Encoding.GetEncoding("UTF-8").GetBytes(fileNameInChar);
            byte[] orderInByte = BitConverter.GetBytes(order);
            byte[] amountInByte = BitConverter.GetBytes(amount);

            commandInByte.CopyTo(packetInByteArray, 0);
            lengthInByte.CopyTo(packetInByteArray, commandInByte.Length);
            fileNameInByte.CopyTo(packetInByteArray, commandInByte.Length + lengthInByte.Length);
            orderInByte.CopyTo(packetInByteArray, commandInByte.Length + lengthInByte.Length + fileNameInByte.Length);
            amountInByte.CopyTo(packetInByteArray, commandInByte.Length + lengthInByte.Length + fileNameInByte.Length + orderInByte.Length);
            fileSegment.CopyTo(packetInByteArray, commandInByte.Length + lengthInByte.Length + fileNameInByte.Length + orderInByte.Length + amountInByte.Length);

            Debug.Log(i);
            Debug.Log(BitConverter.ToString(packetInByteArray));


        }







        if (BluetoothAdapter.isBluetoothEnabled ()) {
			connect ();
		} else {

			//BluetoothAdapter.enableBluetooth(); //you can by this force enabling Bluetooth without asking the user
			statusText.text = "Status : Please enable your Bluetooth";

			BluetoothAdapter.OnBluetoothStateChanged += HandleOnBluetoothStateChanged;
			BluetoothAdapter.listenToBluetoothState (); // if you want to listen to the following two events  OnBluetoothOFF or OnBluetoothON

			BluetoothAdapter.askEnableBluetooth ();//Ask user to enable Bluetooth

		}
	}

	void Start ()
	{
		BluetoothAdapter.OnDeviceOFF += HandleOnDeviceOff;//This would mean a failure in connection! the reason might be that your remote device is OFF

		BluetoothAdapter.OnDeviceNotFound += HandleOnDeviceNotFound; //Because connecting using the 'Name' property is just searching, the Plugin might not find it!.

        BtStruct packet = new BtStruct();
        packet.command = 0x10203040;
        packet.length = sizeof(uint) + sizeof(ushort) * 3;// + 16;
                                                          //packet.context = "abcdefghijklmno";
        packet.category = 0xC2A3;
        packet.function = 0x4CB6;
        packet.context = "abcdefghijklmno";
        Debug.Log(packet.length);
        BinaryFormatter formatter = new BinaryFormatter();
        byte[] packetInByteArray;

        byte[] command = BitConverter.GetBytes(packet.command);
        byte[] length = BitConverter.GetBytes(packet.length);
        byte[] category = BitConverter.GetBytes(packet.category);
        byte[] function = BitConverter.GetBytes(packet.function);

        char[] sentence = packet.context.ToCharArray();
        byte[] context = Encoding.GetEncoding("UTF-8").GetBytes(sentence);
        byte endOfLine = (byte)10;

        packetInByteArray = new byte[command.Length + length.Length + category.Length + function.Length + context.Length + 1];

        command.CopyTo(packetInByteArray, 0);
        length.CopyTo(packetInByteArray, command.Length);
        category.CopyTo(packetInByteArray, command.Length + length.Length);
        function.CopyTo(packetInByteArray, command.Length + length.Length + category.Length);
        context.CopyTo(packetInByteArray, command.Length + length.Length + category.Length + function.Length);
        packetInByteArray[command.Length + length.Length + category.Length + function.Length + context.Length] = endOfLine;

        /*

        UnityEngine.Object obj = Resources.Load("WantSeeYou");

        TextAsset textAsset = Resources.Load("WantSeeYou") as TextAsset;

        Stream stream = File.OpenRead(Application.dataPath + "/Resources/WantSeeYou.sm");

        Debug.Log(stream.Length);

        byte[] streamBytes = new byte[stream.Length];
        stream.Read(streamBytes, 0, streamBytes.Length);

        Debug.Log(BitConverter.ToString(streamBytes));

        string inputString = System.Convert.ToBase64String(streamBytes);

        Debug.Log(inputString);

        //byte[] bytes = textAsset.bytes;

        //Console.WriteLine("   {0}\n", BitConverter.ToString(bytes));
        */


        /*
        using (MemoryStream ms = new MemoryStream())
        {
            formatter.Serialize(ms, packet);
            packetInByteArray = ms.ToArray();
        }
        */
        string converted = "";

        for(int i = 0; i < packetInByteArray.Length; i++)
        {
            converted += packetInByteArray[i].ToString("X2") + " ";
        }

        Debug.Log(converted);
    }

	private void connect ()
	{


		statusText.text = "Status : Trying To Connect 0341";
		

		/* The Property device.MacAdress doesn't require pairing. 
		 * Also Mac Adress in this library is Case sensitive,  all chars must be capital letters
		 */
		device.MacAddress = "B8:27:EB:E5:8D:32";
		
		/* device.Name = "My_Device";
		* 
		* Trying to identefy a device by its name using the Property device.Name require the remote device to be paired
		* but you can try to alter the parameter 'allowDiscovery' of the Connect(int attempts, int time, bool allowDiscovery) method.
		* allowDiscovery will try to locate the unpaired device, but this is a heavy and undesirable feature, and connection will take a longer time
		*/
						
						
		/*
		 * 10 equals the char '\n' which is a "new Line" in Ascci representation, 
		 * so the read() method will retun a packet that was ended by the byte 10. simply read() will read lines.
		 * If you don't use the setEndByte() method, device.read() will return any available data (line or not), then you can order them as you want.
		 */
		//device.setEndByte (10);


		/*
		 * The ManageConnection Coroutine will start when the device is ready for reading.
		 */
		device.ReadingCoroutine = ManageConnection;


		statusText.text = "Status : trying to connect";
		
		device.connect ();

	}


	//############### Handlers/Recievers #####################
	void HandleOnBluetoothStateChanged (bool isBtEnabled)
	{
		if (isBtEnabled) {
			connect ();
			//We now don't need our recievers
			BluetoothAdapter.OnBluetoothStateChanged -= HandleOnBluetoothStateChanged;
			BluetoothAdapter.stopListenToBluetoothState ();
		}
	}

	//This would mean a failure in connection! the reason might be that your remote device is OFF
	void HandleOnDeviceOff (BluetoothDevice dev)
	{
		if (!string.IsNullOrEmpty (dev.Name)) {
			statusText.text = "Status : can't connect to '" + dev.Name + "', device is OFF ";
		} else if (!string.IsNullOrEmpty (dev.MacAddress)) {
			statusText.text = "Status : can't connect to '" + dev.MacAddress + "', device is OFF ";
		}
	}

	//Because connecting using the 'Name' property is just searching, the Plugin might not find it!.
	void HandleOnDeviceNotFound (BluetoothDevice dev)
	{
		if (!string.IsNullOrEmpty (dev.Name)) {
			statusText.text = "Status : Can't find a device with the name '" + dev.Name + "', device might be OFF or not paird yet ";

		} 
	}
	
	public void disconnect ()
	{
		if (device != null)
			device.close ();
	}
	
	//############### Reading Data  #####################
	//Please note that you don't have to use this Couroutienes/IEnumerator, you can just put your code in the Update() method
	IEnumerator  ManageConnection (BluetoothDevice device)
	{
		statusText.text = "Status : Connected & Can read";

        int count = 0;

        TextAsset textAsset = Resources.Load<TextAsset>("aaa");

        if (textAsset == null)
        {
            Debug.Log("Resource load failed.");
            statusText.text = "Status : load file failed";
        }

        byte[] bytes = textAsset.bytes;

        Debug.Log(BitConverter.ToString(bytes));

        Console.WriteLine("   {0}\n", BitConverter.ToString(bytes));

        // ----------------------------------------------------------------------
        /*
        Stream stream = File.OpenRead(Application.dataPath + "/Resources/WantSeeYou.sm");

        byte[] streamBytes = new byte[stream.Length];
        stream.Read(streamBytes, 0, streamBytes.Length);

        Debug.Log(streamBytes.Length);

        Debug.Log(BitConverter.ToString(streamBytes));

        string inputString = System.Convert.ToBase64String(streamBytes);

        Debug.Log(inputString);
        */
        // -----------------------------------------------------------------------

        int tempPosition = 0;

        int tempRun = 0;

        while (device.IsReading) {
			if (device.IsDataAvailable) {
				byte [] msg = device.read ();//because we called setEndByte(10)..read will always return a packet excluding the last byte 10.
				
				if (msg != null && msg.Length > 0) {
					string content = System.Text.ASCIIEncoding.ASCII.GetString (msg);
					statusText.text = "MSG : " + content + "[" + count.ToString() + "]";

                    count++;
				}
            }

            if (device != null/* && tempRun < 63*/)
            {
                statusText.text = "Status : copying segment";

                ushort segmentSize = (ushort)(bytes.Length - tempPosition > 512 ? 512 : bytes.Length - tempPosition);

                uint command = 0x000803;
                ushort length = (ushort)(sizeof(uint) + sizeof(ushort) + sizeof(byte) * 16 + sizeof(ushort) * 2 + sizeof(byte) * segmentSize);
                string fileName = "aaaaaaaaaaaaa.sm";
                ushort order = (ushort)tempRun;
                ushort amount = (ushort)(bytes.Length / 512 + (bytes.Length % 512 > 0 ? 1 : 0));
                byte[] fileSegment = new byte[segmentSize];
                Array.Copy(bytes, tempRun * 512, fileSegment, 0, segmentSize);

                byte[] packetInByteArray = new byte[length];

                byte[] commandInByte = BitConverter.GetBytes(command);
                byte[] lengthInByte = BitConverter.GetBytes(length);
                char[] fileNameInChar = fileName.ToCharArray();
                byte[] fileNameInByte = Encoding.GetEncoding("UTF-8").GetBytes(fileNameInChar);
                byte[] orderInByte = BitConverter.GetBytes(order);
                byte[] amountInByte = BitConverter.GetBytes(amount);

                commandInByte.CopyTo    (packetInByteArray, 0);
                lengthInByte.CopyTo     (packetInByteArray, commandInByte.Length);
                fileNameInByte.CopyTo   (packetInByteArray, commandInByte.Length + lengthInByte.Length);
                orderInByte.CopyTo      (packetInByteArray, commandInByte.Length + lengthInByte.Length + fileNameInByte.Length);
                amountInByte.CopyTo     (packetInByteArray, commandInByte.Length + lengthInByte.Length + fileNameInByte.Length + orderInByte.Length);
                fileSegment.CopyTo      (packetInByteArray, commandInByte.Length + lengthInByte.Length + fileNameInByte.Length + orderInByte.Length + amountInByte.Length);

                statusText.text = "Status : start sending" + tempRun;




                device.send(packetInByteArray);
                tempRun++;

                
            }


            if (device != null && count < 2 && false)
            {

                BtStruct packet = new BtStruct();
                //packet.command = 0x00000313;
                packet.command = 0x00000807;
                //packet.length = sizeof(uint) + sizeof(ushort) * 3 + 22 + 1; // end of line算一個byte
                packet.length = sizeof(uint) + sizeof(ushort) * 3 + 25 + 1; // end of line算一個byte song
                packet.order = 0;
                packet.amount = 1;
                //packet.context = "{\"Instrument\":\"piano\"}";
                packet.context = "{\"FileName\":\"AnySong.sm\"}";
                //packet.length = sizeof(uint) + sizeof(ushort) * 3 + ushort(packet.context.Length) + 1;

                byte[] packetInByteArray;

                byte[] command = BitConverter.GetBytes(packet.command);
                byte[] length = BitConverter.GetBytes(packet.length);
                byte[] order = BitConverter.GetBytes(packet.order);
                byte[] amount = BitConverter.GetBytes(packet.amount);

                char[] sentence = packet.context.ToCharArray();
                byte[] context = Encoding.GetEncoding("UTF-8").GetBytes(sentence);
                byte endOfLine = (byte)10;


                packetInByteArray = new byte[command.Length + length.Length + order.Length + amount.Length + context.Length + 1];

                command.CopyTo(packetInByteArray, 0);
                length.CopyTo(packetInByteArray, command.Length);
                order.CopyTo(packetInByteArray, command.Length + length.Length);
                amount.CopyTo(packetInByteArray, command.Length + length.Length + order.Length);
                context.CopyTo(packetInByteArray, command.Length + length.Length + order.Length + amount.Length);
                packetInByteArray[command.Length + length.Length + order.Length + amount.Length + context.Length] = endOfLine;

                device.send(packetInByteArray);

                break;

                //device.send(System.Text.Encoding.ASCII.GetBytes("received " + count.ToString()));


                /*
                BtStruct packet = new BtStruct();
                packet.command = 0x10203040;
                packet.length = sizeof(uint) + sizeof(ushort) * 3 + 16;
                packet.category = 0xC2A3;
                packet.function = 0x4CB6;
                packet.context = "abcdefghijklmno";
                Debug.Log(packet.length);
                byte[] packetInByteArray;

                byte[] command = BitConverter.GetBytes(packet.command);
                byte[] length = BitConverter.GetBytes(packet.length);
                byte[] category = BitConverter.GetBytes(packet.category);
                byte[] function = BitConverter.GetBytes(packet.function);

                char[] sentence = packet.context.ToCharArray();
                byte[] context = Encoding.GetEncoding("UTF-8").GetBytes(sentence);
                byte endOfLine = (byte)10;

                packetInByteArray = new byte[command.Length + length.Length + category.Length + function.Length + context.Length + 1];

                command.CopyTo(packetInByteArray, 0);
                length.CopyTo(packetInByteArray, command.Length);
                category.CopyTo(packetInByteArray, command.Length + length.Length);
                function.CopyTo(packetInByteArray, command.Length + length.Length + category.Length);
                context.CopyTo(packetInByteArray, command.Length + length.Length + category.Length + function.Length);
                packetInByteArray[command.Length + length.Length + category.Length + function.Length + context.Length] = endOfLine;


                device.send(packetInByteArray);
                */
            }

            yield return null;
		}

		statusText.text = "Status : Done Reading";
	}
	
	
	//############### Deregister Events  #####################
	void OnDestroy ()
	{
		BluetoothAdapter.OnDeviceOFF -= HandleOnDeviceOff;
		BluetoothAdapter.OnDeviceNotFound -= HandleOnDeviceNotFound;
		
	}
	
}
