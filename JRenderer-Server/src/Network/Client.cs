using System.Collections;
using System.Collections.Generic;
using System;
using System.Net;
using System.Net.Sockets;
/// <summary>
/// 对应每个用户，控制TCP/UDP信号连接
/// </summary>
public class Client 
{
    public static int dataBufferSize = 4096;
    public int id;
    public TCP tcp;
    public UDP udp;
    public Client(int _clientid)
    {
        id = _clientid;
        tcp = new TCP(id);
        udp = new UDP(id);
    }
    public class TCP
    {
        public TcpClient socket;
        private readonly int id;
        private NetworkStream stream;
        private byte[] receiveBuffer;
        private Packet receiveData;
        public TCP(int _id)
        {
            id = _id;
        }
        public void Connect(TcpClient _socket)
        {
            socket = _socket;
            socket.ReceiveBufferSize = dataBufferSize;
            socket.SendBufferSize = dataBufferSize;

            stream = socket.GetStream();
            receiveData = new Packet();
            receiveBuffer = new byte[dataBufferSize];
            stream.BeginRead(receiveBuffer, 0, dataBufferSize, ReceiveCallback, null);

            ServerSend.Welcome(id, "Welcome to the AOH server!");
        }

        public void SendData(Packet _packet)
        {
            try
            {
                if (socket != null)
                {
                    stream.BeginWrite(_packet.ToArray(), 0, _packet.Length(), null, null);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error Sending data to player {id} via TCP:{ex} ");
            }
        }
        private void ReceiveCallback(IAsyncResult result)
        {
            try
            {
                int byteLength = stream.EndRead(result);
                if (byteLength <= 0)
                {
                    Server.clients[id].Disconnect();
                    return;
                }
                byte[] data = new byte[byteLength];
                Array.Copy(receiveBuffer, data, byteLength);
                receiveData.Reset(HandleData(data));

                stream.BeginRead(receiveBuffer, 0, dataBufferSize, ReceiveCallback, null);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error receive TCP data: {ex}");
                Server.clients[id].Disconnect();
            }
        }
        private bool HandleData(byte[] _data)
        {
            int _packetLength = 0;
            receiveData.SetBytes(_data);
            if (receiveData.UnreadLength() >= 4)
            {
                _packetLength = receiveData.ReadInt();
                if (_packetLength <= 0)
                {
                    return true;
                }
            }
            while (_packetLength > 0 && _packetLength <= receiveData.UnreadLength())
            {
                byte[] _packetBytes = receiveData.ReadBytes(_packetLength);
                ThreadManager.ExecuteOnMainThread(() =>
                {
                    using (Packet _packet = new Packet(_packetBytes))
                    {
                        int _packetId = _packet.ReadInt();
                        Server.packetHandlers[_packetId](id, _packet);
                    }
                });
                _packetLength = 0;
                if (receiveData.UnreadLength() >= 4)
                {
                    _packetLength = receiveData.ReadInt();
                    if (_packetLength <= 0)
                    {
                        return true;
                    }
                }
            }
            if (_packetLength <= 1)
            {
                return true;
            }
            return false;
        }

        public void Disconnect()
        {
            socket.Close();
            stream = null;
            receiveData = null;
            receiveBuffer = null;
            socket = null;
        }
    }
    public class UDP
    {
        public IPEndPoint endPoint;
        private int id;
        public UDP(int _id)
        {
            id = _id;
        }
        public void Connect(IPEndPoint _endPoint)
        {
            endPoint = _endPoint;
        }
        public void SendData(Packet _packet)
        {
            Server.SendUDPData(endPoint, _packet);
        }
        public void HandleData(Packet _packetData)
        {
            int _packetLength = _packetData.ReadInt();
            byte[] _packetBytes = _packetData.ReadBytes(_packetLength);

            ThreadManager.ExecuteOnMainThread(() =>
            {
                using (Packet _packet = new Packet(_packetBytes))
                {
                    int _packetId = _packet.ReadInt();
                    Server.packetHandlers[_packetId](id, _packet);
                }
            });
        }
        public void Disconnect()
        {
            endPoint = null;
        }
    }
    private void Disconnect()
    {
        Console.WriteLine($"{tcp.socket.Client.RemoteEndPoint} has disconnected.");
        tcp.Disconnect();
        udp.Disconnect();
    }
}
