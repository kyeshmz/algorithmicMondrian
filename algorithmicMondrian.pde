import java.io.ByteArrayOutputStream;
import java.util.zip.DataFormatException;
import java.util.zip.Deflater;
import java.util.zip.Inflater;
import java.util.zip.CRC32;
import javax.xml.bind.DatatypeConverter;
import oscP5.*;
import netP5.*;

OscP5 oscP5;
int screenShotNum;
int _screenShotNum;
int pnGlitchNum;
int _pnGlitchNum;
 
static final int FT_NONE =0;
static final int FT_SUB=1;
static final int FT_UP=2;
static final int FT_AVRG=3;
static final int FT_PAETH=4;
 
static final String INTIAL_INPUT_FILE_NAME="bin/data/screenshot.png";
static final String OUTPUT_FILE_NAME_PREFIX="out_";
static final int APP_FRAME_RATE=10;
static final int INTERVAL=1;
static final int FILTER_CHANGE_FREQUENCY=200;
static final int FILTER_CHANGE_PROBABILITY=100;
static final int[] FILTER_CANDIDATES={FT_NONE,FT_SUB,FT_UP,FT_AVRG,FT_PAETH};
 
static final String PNG_FILE_SIGNATURE="89504e470d0a1a0a";
static final String HEX_IHDR="49484452";
static final String HEX_IDAT="49444154";
static final String HEX_IEND="49454e44";
static final int IHDR=1;
static final int IDAT=2;
static final int IEND=3;
static final HashMap CHANK_TYPE_MAP=new HashMap(){{
  put(HEX_IHDR,IHDR);
  put(HEX_IDAT,IDAT);
  put(HEX_IEND,IEND);
}};
 
static final int FILE_SIGNATURE_SIZE=8;
static final int LENGTH_SIZE=4;
static final int CHANK_TYPE_SIZE=4;
static final int CRC_SIZE=4;

static final int IHDR_DATA_SIZE=13;
static final int IHDR_IW_SIZE=4;
static final int IHDR_IH_SIZE=4;
static final int IHDR_BD_SIZE=1;
static final int IHDR_CT_SIZE=1;
static final int IHDR_CM_SIZE=1;
static final int IHDR_FM_SIZE=1;
static final int IHDR_IM_SIZE=1;

static final int DECOMPRESS_UNIT=1024;
static final int COMPRESS_UNIT=1024;

static final int BPP=3;

static final int IHDR_CHANK_SIZE=LENGTH_SIZE+CHANK_TYPE_SIZE+IHDR_DATA_SIZE+CRC_SIZE;
static final int IEND_CHANK_SIZE=LENGTH_SIZE+CHANK_TYPE_SIZE+CRC_SIZE;

byte[] signature=new byte[FILE_SIGNATURE_SIZE];
byte[] ihdr=new byte[IHDR_CHANK_SIZE];
byte[] idat=null;
byte[] iend=new byte[IEND_CHANK_SIZE];
 
HeaderInfo imageInfo=null;
PImage dispImg=null;
String currentFileName="";
 
void setup(){
  byte b[]=loadBytes(INTIAL_INPUT_FILE_NAME); 
  int idx=0;
 
  arrayCopy(b,idx,signature,0,FILE_SIGNATURE_SIZE);
  String str=readAsHexString(signature,0,FILE_SIGNATURE_SIZE);
  if(!isPng(str)){
    println("ERROR: This file is broken, or not PNG.");
    return;
  }else{
    idx+=FILE_SIGNATURE_SIZE;
  }
 
  int length=-1;
  int iChankType=-1;
  while(idx>=0){
    length=readChankDataLength(b,idx);
    iChankType=readChankType(b,idx+LENGTH_SIZE);
    switch(iChankType){
    case IHDR:
      arrayCopy(b,idx,ihdr,0,IHDR_CHANK_SIZE);
      imageInfo=readIHDR(ihdr,0);
      break;
    case IDAT:
      if(idat==null){
        idat=readIDAT(b,idx,length);
      }else{
        idat=concat(idat,readIDAT(b,idx,length));
      }
      break;
    case IEND:
    arrayCopy(b,idx,iend,0,IEND_CHANK_SIZE);
      idx=-1;
      break;
    default:
      break;
    }

    if(idx>=0){
      int chankSize=LENGTH_SIZE+CHANK_TYPE_SIZE+length+CRC_SIZE;
      idx+=chankSize;
    }
  }
  
  //size(imageInfo.imageHeight,imageInfo.imageWidth);
  size(1,1);
  frameRate(APP_FRAME_RATE);
  createPNG(idat);
  background(0);
  dispImg=loadImage(currentFileName);
  
  screenShotNum=-1;
  _screenShotNum=-1;
  pnGlitchNum=-1;
  _pnGlitchNum=-1;
  
  oscP5=new OscP5(this,8000);
  oscP5.plug(this,"getData","/test");
}

public void getData(int x,int y){
  screenShotNum=x;
  pnGlitchNum=y;
}
 
void draw(){
  if(screenShotNum!=_screenShotNum){
    signature=new byte[FILE_SIGNATURE_SIZE];
    ihdr=new byte[IHDR_CHANK_SIZE];
    idat=null;
    iend=new byte[IEND_CHANK_SIZE];

    byte[] b=loadBytes(INTIAL_INPUT_FILE_NAME); 
    int idx=0;
 
    arrayCopy(b,idx,signature,0,FILE_SIGNATURE_SIZE);
    String str=readAsHexString(signature,0,FILE_SIGNATURE_SIZE);
    if(!isPng(str)){
      println("ERROR: This file is broken, or not PNG.");
      return;
    }else{
      idx+=FILE_SIGNATURE_SIZE;
    }
 
    int length=-1;
    int iChankType=-1;
    while(idx>=0){
      length=readChankDataLength(b,idx);
      iChankType=readChankType(b,idx+LENGTH_SIZE);
      switch(iChankType){
      case IHDR:
        arrayCopy(b,idx,ihdr,0,IHDR_CHANK_SIZE);
        imageInfo=readIHDR(ihdr,0);
        break;
      case IDAT:
        if (idat==null) {
          idat=readIDAT(b,idx,length);
        }else{
          idat=concat(idat,readIDAT(b,idx,length));
        }
        break;
      case IEND:
        arrayCopy(b,idx,iend,0,IEND_CHANK_SIZE);
        idx=-1;
        break;
      default:
        break;
      }
      if(idx>=0){
        int chankSize=LENGTH_SIZE+CHANK_TYPE_SIZE+length+CRC_SIZE;
        idx+=chankSize;
      }
    }
    createPNG(idat);
    _screenShotNum=screenShotNum;
  }
  if(pnGlitchNum!=_pnGlitchNum){
    glitch();
    _pnGlitchNum=pnGlitchNum;
  }
  //dispImg=loadImage(currentFileName);
  //image(dispImg, 0, 0);
}
 
byte[] to4Bytes(int i){
  byte[] b=new byte[4];
  b[3]=(byte)(0x000000ff&(i));
  b[2]=(byte)(0x000000ff&(i>>>8));
  b[1]=(byte)(0x000000ff&(i>>>16));
  b[0]=(byte)(0x000000ff&(i>>>24));
  return b;
}
 
String readAsHexString(byte[] bytes,int init_idx,int length){
  StringBuffer strbuf=new StringBuffer(length);  
  for(int i=init_idx;i<(init_idx+length);i++){ 
    int bt=bytes[i]&0xff;
    if(bt<0x10)strbuf.append("0");
    strbuf.append(Integer.toHexString(bt));
  }
  return strbuf.toString(); 
}

int readAsInt(byte[] bytes,int init_idx,int length){
  String hexStr=readAsHexString(bytes,init_idx,length);
  return Integer.parseInt(hexStr,16);
}

long readAsLong(byte[] bytes,int init_idx,int length){
  String hexStr=readAsHexString(bytes,init_idx,length);
  return Long.parseLong(hexStr,16);
}

boolean isPng(String signatureHexStr){
  return signatureHexStr.equals(PNG_FILE_SIGNATURE);
}

int readChankDataLength(byte[] bytes,int idx){
  return readAsInt(bytes,idx,LENGTH_SIZE);
}

int readChankType(byte[] bytes,int idx){
  String sChankType=readAsHexString(bytes,idx,CHANK_TYPE_SIZE);
  int res=-1;
  try{
    res=(Integer)CHANK_TYPE_MAP.get(sChankType);
  }catch(NullPointerException e){
  }
  return res;
}

int getChankDataPosition(int idx){
  return idx+LENGTH_SIZE+CHANK_TYPE_SIZE;
}

HeaderInfo readIHDR(byte[] bytes,int idx){
  byte[] chankData=new byte[IHDR_DATA_SIZE];
  int dataPos=getChankDataPosition(idx);
  arrayCopy(bytes,dataPos,chankData,0,IHDR_DATA_SIZE);
  long crc=readAsLong(bytes,dataPos+IHDR_DATA_SIZE,CRC_SIZE);
  boolean valid=verifyCRC(DatatypeConverter.parseHexBinary(HEX_IHDR),chankData,crc);
  if(!valid)println("WARN: IHDR CRC is not valid.");
  return new HeaderInfo(chankData);
}

byte[] readIDAT(byte[] bytes,int idx,int length){
  byte[] chankData=new byte[length];
  int dataPos=getChankDataPosition(idx);
  arrayCopy(bytes,dataPos,chankData,0,length);
  long crc=readAsLong(bytes,dataPos+length,CRC_SIZE);
  boolean valid=verifyCRC(DatatypeConverter.parseHexBinary(HEX_IDAT),chankData,crc);
  if(!valid)println("WARN: IDAT CRC is not valid.");
  return chankData;
}

long calculateChankCRC(byte[] typeBytes,byte[] data){
  CRC32 crc32=new CRC32();
  crc32.update(typeBytes);
  crc32.update(data);
  return crc32.getValue();
}

boolean verifyCRC(byte[] typeBytes,byte[] data,long crc){
  long calculated=calculateChankCRC(typeBytes,data);
  return (calculated==crc);
}

byte[] decompress(byte[] bytes){
  ByteArrayOutputStream result=new ByteArrayOutputStream();
  Inflater decompresser=new Inflater();
  decompresser.setInput(bytes);
 
  while(!decompresser.finished()){
    try{
      byte[] resultBuf=new byte[DECOMPRESS_UNIT];
      int resultLength=decompresser.inflate(resultBuf);
      result.write(resultBuf,0,resultLength);
    }catch(DataFormatException e){
      println("ERROR: Decompress error occurred.");
    }
  }
  decompresser.end();
  return result.toByteArray();
}

byte[] compress(byte[] bytes){
  Deflater compresser=new Deflater();
  compresser.setInput(bytes);
  compresser.finish();
 
  ByteArrayOutputStream result=new ByteArrayOutputStream();
 
  while(!compresser.finished()){
    byte[] resultBuf=new byte[DECOMPRESS_UNIT];
    int resultLength=compresser.deflate(resultBuf);
    result.write(resultBuf,0,resultLength);
  }
  return result.toByteArray();
}
 
void createPNG(byte[] bIdatData){
  byte[] bIdatLength=to4Bytes(bIdatData.length);
  byte[] bIdatType=DatatypeConverter.parseHexBinary(HEX_IDAT);
  byte[] bIdatCRC=to4Bytes((int)calculateChankCRC(bIdatType,bIdatData));
  byte[] idatChank=concat(concat(concat(bIdatLength,bIdatType),bIdatData),bIdatCRC);
  byte[] output=concat(concat(concat(signature,ihdr),idatChank),iend);
 
  String outputFileName="bin/data/pnglitch.png";
  saveBytes(outputFileName,output);
  currentFileName=outputFileName;
}
 
void glitch() {
  byte[] imageData=decompress(idat);
 
  int filterType=0;
  int changedFilterType=0;
  for(int i=0;i<imageInfo.imageHeight;i++){  
    int rowHead=(i*(imageInfo.imageWidth*BPP+1));
    filterType=readAsInt(imageData,rowHead,1);
    if((i%FILTER_CHANGE_FREQUENCY)==0)changedFilterType=FILTER_CANDIDATES[(int)random(FILTER_CANDIDATES.length)];
    filterType=(random(100)<FILTER_CHANGE_PROBABILITY)?changedFilterType:filterType;
    imageData[rowHead]=((Integer)filterType).byteValue();
  }
  byte[] glitchedImageData=compress(imageData);
  createPNG(glitchedImageData);
}

class HeaderInfo{
  int imageWidth;
  int imageHeight;
  int bitDepth;
  int colorType;
  int compressionMethod;
  int filterMethod;
  int interlaceMethod;
 
  HeaderInfo(byte[] data){
    int idx=0;
    imageWidth=readAsInt(data,idx,IHDR_IW_SIZE);
    imageHeight=readAsInt(data,idx += IHDR_IW_SIZE,IHDR_IH_SIZE);
    bitDepth=readAsInt(data,idx+=IHDR_IH_SIZE,IHDR_BD_SIZE);
    colorType=readAsInt(data,idx+=IHDR_BD_SIZE,IHDR_CT_SIZE);
    compressionMethod=readAsInt(data,idx+=IHDR_CT_SIZE,IHDR_CM_SIZE);
    filterMethod=readAsInt(data,idx+=IHDR_CM_SIZE,IHDR_FM_SIZE);
    interlaceMethod=readAsInt(data,idx+=IHDR_FM_SIZE,IHDR_IM_SIZE);
  }
}
