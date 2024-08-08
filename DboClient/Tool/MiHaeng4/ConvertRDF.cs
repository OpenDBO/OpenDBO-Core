using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;
using System.Windows.Forms;

namespace MiHaeng4
{
    class ConvertRDF
    {
        private enum NodeType
        {
            NODE_TYPE_ID,
            NODE_TYPE_TEXT,
            NODE_TYPE_ETC,
        };

        private NodeType nodeType;

        public ConvertRDF()
        {
            nodeType = new NodeType();
            nodeType = NodeType.NODE_TYPE_ETC;
        }

        public bool ConvertToRDF(string[] xmlFiles, string rdfFileName)
        {
            FileStream fs = new FileStream(rdfFileName, FileMode.Create);
            BinaryWriter bwRDF = new BinaryWriter(fs, Encoding.Unicode);
            UnicodeEncoding unicode = new UnicodeEncoding();

            MH4Global.pgbXML.Maximum = xmlFiles.Length;
            MH4Global.pgbXML.Value = 0;

            // 서버의 요청으로 인해서 더미 1 바이트를 넣어준다
            bwRDF.Write(new byte[1]);

            foreach(string xmlFile in xmlFiles)
            {
                string nakedFileName = Path.GetFileName(xmlFile);
                MH4Global.lbXML.Text = "RDF Converting... : " + nakedFileName;
                MH4Global.lbXML.Update();

                XmlTextReader reader = new XmlTextReader(xmlFile);
                // XML의 헤더파일 부분은 그냥 넘긴다. (룰에 안따르고 있기때문)
                for (int i = 0; i < 12; ++i)
                {
                    reader.Read();
                }

                while (reader.Read())
                {
                    reader.MoveToContent();

                    if (reader.NodeType == XmlNodeType.Element)
                    {
                        if (reader.Name == "F1")
                        {
                            nodeType = NodeType.NODE_TYPE_ID;
                        }
                        else if (reader.Name == "F2")
                        {
                            nodeType = NodeType.NODE_TYPE_TEXT;
                        }
                        else
                        {
                            nodeType = NodeType.NODE_TYPE_ETC;
                        }
                    }
                    else if (reader.NodeType == XmlNodeType.Text)
                    {
                        try
                        {
                            if (nodeType == NodeType.NODE_TYPE_ID)
                            {
                                bwRDF.Write(Convert.ToUInt32(reader.Value));
                            }
                            else if (nodeType == NodeType.NODE_TYPE_TEXT)
                            {
                                bwRDF.Write(Convert.ToUInt16(reader.Value.Length));
                                Byte[] byteText = unicode.GetBytes(reader.Value);
                                bwRDF.Write(byteText, 0, byteText.Length);
                            }
                        }
                        catch
                        {
                            MessageBox.Show("XML 파일을 파싱중에 에러가 발생하였습니다." + xmlFile + "(" + reader.LineNumber + ")");
                            break;
                        }
                    }
                }

                reader.Close();

                MH4Global.pgbXML.Value += 1;
            }

            bwRDF.Close();
            fs.Close();

            //fs = new FileStream(rdfFileName, FileMode.Open, FileAccess.Read);
            //BinaryReader br = new BinaryReader(fs);
            //UInt32 id = br.ReadUInt32();
            //UInt16 lenValue = br.ReadUInt16();
            //string val = br.ReadString();

            return true;           
        }       
    }
}
