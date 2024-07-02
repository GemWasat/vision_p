#pragma once
#include <iostream>
#include <libpq-fe.h>
#include <string>

#include <fstream>
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/DeleteObjectsRequest.h>

#include "RobotInfo.h"

using namespace std;
using namespace Aws;
using namespace Aws::Client;
using namespace Aws::S3;
using namespace Aws::S3::Model;

class AWS
{
private:
    // 데이터베이스 연결 정보 설정
    const char* conninfo = "host=baxdatabase.clyecqs0oin9.ap-northeast-2.rds.amazonaws.com dbname=boxdata user=boxuser password=qwer1234";
    const String bucketName = "box-s3-buket";
    // PostgreSQL 연결 객체 생성
    PGconn* conn = PQconnectdb(conninfo);

    //s3
    String m_region;// 지역 설정
    ClientConfiguration m_clientConfig;// 클라이언트 설정
    SDKOptions options;// AWS S3 서비스 사용을 위한 SDK 초기화

public:
	AWS();
	~AWS();

	// RDS
    bool RDSinsertData(const char* columnname, const char* inputData, const char* tablename = "thing");// 데이터 삽입 함수
    bool RDSselectData(const char* whatGet, const char* whereData);// 데이터 출력 함수
    bool RDSupdateData(const char* columnname, const char* reviseData, const char* id, const char* standard_columnname = "id", const char* tablename = "thing");// 데이터 수정 함수
    bool RDSdeleteData(const char* columnname, const char* deleteline, const char* tablename = "thing");// 데이터 삭제 함수
    bool RDScheckDataExists(const char* columnname, const char* findwhat, const char* tablename = "thing");// 데이터 존재 확인 함수
    bool RDSinserts3Data(string date);// S3 데이터 삽입 함수
    vector<AWSLIST> AWS::RDSjoinData();// 데이터 테이블 연동 함수
    bool RDSckeckConnection();// 연결 확인 함수
    //void RDSconnectionEnd();// 연결 종료 함수

    //s3
    //bool JoinS3();// S3연결 함수
    bool PutObject(const String& fileName, string date);// 오브젝트 추가 함수
    bool GetObject(const String& objectKey);// 오브젝트 가져오기 함수
    bool DeleteObjects(const std::vector<Aws::String>& objectKey);// 오브젝트 삭제 함수
    void S3connectionEnd();// S3 연결 종료 함수

    //한번에 모든 테이블 데이터 올리기
    void Allinput(string date, const char* inputData, const String & fileName, const char* columnname = "color, faulty", const char* tablename = "thing");
    

    void test(string testid);
    void testinput(string date, const char* inputData, const String& fileName, const char* columnname = "color, faulty", const char* tablename = "thing");

    //한번에 모든 테이블의 원하는 행 데이터 지우기
    void AlldeleteData(const char* deleteline, const char* columnname="id", const char* tablename = "thing", const char* tablename2 = "s3data");
};

