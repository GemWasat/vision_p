
#include "pch.h"
#include "AWS.h"
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

AWS::AWS() : m_clientConfig(ClientConfiguration())
{
    m_clientConfig.region = "ap-northeast-2";
}

AWS::~AWS()
{
}

////////////////////////////////////////////////////////////
////////////////////RDS/////////////////////////////////////
////////////////////////////////////////////////////////////

// 데이터 삽입 함수
bool AWS::RDSinsertData(const char* columnname, const char* inputData, const char* tablename)
{
    // 삽입할 데이터를 포함한 SQL 쿼리 생성
    string insert_query = "INSERT INTO public." + string(tablename) + " (" + string(columnname) + ") VALUES " + string(inputData);
    //string insert_query = "INSERT INTO public.thing (id, color, faulty) VALUES ('3', 'red', 'false')";
    
    // 쿼리 실행
    PGresult* res = PQexec(conn, insert_query.c_str());
    
    // 쿼리 실행 결과 확인
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        // 실패 시 에러 메시지 출력
        cerr << "Insertion failed: " << PQerrorMessage(conn) << endl;
        return false;
    }

    PQclear(res);// 결과 클리어

    return true;// 성공 여부 반환
}

// 데이터 출력 함수
bool AWS::RDSselectData(const char* whatGet, const char* whereData)
{
    // 선택할 데이터를 포함한 SQL 쿼리 생성
    string select_query = "SELECT " + string(whatGet) + " FROM " + string(whereData);
    //  string select_query = "SELECT * FROM thing";

    // 쿼리 실행
    PGresult* res = PQexec(conn, select_query.c_str());
    
    // 쿼리 실행 결과 확인
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);// 쿼리 결과로부터 행
        int columns = PQnfields(res); // 가져온 열의 수
        
        // 결과 출력
        for (int i = 0; i < rows; ++i) {
            // 실제 가져온 열의 수만큼만 출력
            for (int j = 0; j < columns; ++j) {
                cout << PQgetvalue(res, i, j) << "\t";
            }
            cout << endl;
        }
    }
    else {
        // 실패 시 에러 메시지 출력
        cerr << "Selection failed: " << PQerrorMessage(conn) << endl;
        return false;
    }
    PQclear(res);// 결과 클리어
    return true;// 성공 여부 반환
}

// 데이터 수정 함수
bool AWS::RDSupdateData(const char* columnname, const char* reviseData, const char* id, const char* standard_columnname, const char* tablename)
{
    // 수정할 데이터를 포함한 SQL 쿼리 생성
    string update_query = "UPDATE public." + string(tablename) + " SET " + string(columnname) + " = \'" + string(reviseData) + "\' WHERE " + string(standard_columnname) + " = \'" + string(id) + "\'";
    //string update_query = "UPDATE public.thing SET color = 'red' WHERE id = '1'";

    // 쿼리 실행
    PGresult* res = PQexec(conn, update_query.c_str());

    // 쿼리 실행 결과 확인
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        // 실패 시 에러 메시지 출력
        cerr << "Update failed: " << PQerrorMessage(conn) << endl;
        return false;
    }
    PQclear(res);// 결과 클리어

    return true;// 성공 여부 반환
}

// 데이터 삭제 함수
bool AWS::RDSdeleteData(const char* columnname, const char* deleteline, const char* tablename)
{
    // 삭제할 데이터를 포함한 SQL 쿼리 생성
    string delete_query = "DELETE FROM public." + string(tablename) + " WHERE " + string(columnname) + " = " + string(deleteline);
    //  string delete_query = "DELETE FROM public.thing WHERE id = '1'";

    // 쿼리 실행
    PGresult* res = PQexec(conn, delete_query.c_str());
    
    if (res == NULL)
    {
        return false;
    }

    // 쿼리 실행 결과 확인
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        // 실패 시 에러 메시지 출력
        cerr << "Deletion failed: " << PQerrorMessage(conn) << endl;
        return false;
    }
    PQclear(res);// 결과 클리어
    return true;// 성공 여부 반환
}

// 데이터 존재 확인 함수
bool AWS::RDScheckDataExists(const char* columnname, const char* findwhat, const char* tablename)
{
    // 데이터 존재 여부를 확인하는 SQL 쿼리 생성
    string select_query = "SELECT EXISTS (SELECT 1 FROM " + string(tablename) + " WHERE " + string(columnname) + " = \'" + string(findwhat) + "\')";
    //  string select_query = "SELECT EXISTS (SELECT 1 FROM thing WHERE color = 'a')";

    PGresult* res = PQexec(conn, select_query.c_str());// 쿼리 실행
    
    // 쿼리 실행 결과 확인
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        if (PQntuples(res) == 1) {
            // 첫 번째 행의 첫 번째 열의 값을 가져옴 (결과는 boolean 형태)
            bool exists = (PQgetvalue(res, 0, 0)[0] == 't'); // 't'는 true, 'f'는 false
            PQclear(res);// 결과 클리어
            return exists;
        }
    }
    PQclear(res);// 결과 클리어
    return false;// 데이터가 존재하지 않거나 쿼리가 실패한 경우 false 반환
}

// S3 데이터 삽입 함수
bool AWS::RDSinserts3Data(string date)
{
    // 최근 항목의 id를 가져오는 SQL 쿼리 생성
    string select_query = "SELECT id FROM (SELECT * FROM thing ORDER BY id DESC LIMIT 1)";
    //  string select_query = "SELECT * FROM thing";

    PGresult* res1 = PQexec(conn, select_query.c_str());// 쿼리 실행

    // 쿼리 실행 결과 확인
    if (PQresultStatus(res1) == PGRES_TUPLES_OK) {
        // 가져온 id를 사용하여 S3 데이터 삽입 쿼리 생성
        string insert_query = "INSERT INTO public.s3data (id, date, url) VALUES (" + string(PQgetvalue(res1,0,0)) + ",\'"+ date +"\',\'https://box-s3-buket.s3.ap-northeast-2.amazonaws.com/" + string(PQgetvalue(res1, 0, 0)) + ".jpg\')";
        //string insert_query = "INSERT INTO public.thing (id, date, url) VALUES ('3', '2024-05-22 17:55:52', '1234.jpg')";
        
        PGresult* res = PQexec(conn, insert_query.c_str());// 삽입 쿼리 실행
        
        // 쿼리 실행 결과 확인
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            // 실패 시 에러 메시지 출력
            cerr << "Insertion failed: " << PQerrorMessage(conn) << endl;
            return false;
        }
        PQclear(res);// 결과 클리어
    }
    PQclear(res1);// 첫 번째 쿼리 결과 클리어

    return true;// 성공 여부 반환
}

// 데이터 테이블 연동 함수
vector<AWSLIST> AWS::RDSjoinData()
{
    vector<AWSLIST> listvector;

    // JOIN 쿼리 생성
    string join_query = "SELECT a.id, a.color, a.faulty, b.date, b.url FROM thing AS a LEFT OUTER JOIN s3data AS b ON a.id = b.id ORDER BY id DESC";

    // 쿼리 실행
    PGresult* res = PQexec(conn, join_query.c_str());

    // 쿼리 실행 결과 확인
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);// 가져온 행의 수
        int columns = PQnfields(res); // 가져온 열의 수
        for (int i = 0; i < rows; ++i) {
            AWSLIST row; // 각 행을 저장할 벡터

            // 실제 가져온 각각의 열을 저장
            row.id = PQgetvalue(res, i, 0);
            row.color = PQgetvalue(res, i, 1);

            row.faulty = PQgetvalue(res, i, 2);
            if (row.faulty == "t") {
                row.faulty = "Faulty";
            }
            else if(row.faulty == "f") {
                row.faulty = "OK";
            }

            row.date = PQgetvalue(res, i, 3);
            row.url = PQgetvalue(res, i, 4);

            listvector.push_back(row); // 행 벡터를 2차원 벡터에 추가
        }
        
        PQclear(res);// 결과 클리어
        
        return listvector;

        //// 데이터를 출력해 봅니다.
        //for (const auto& row : listvector) {
        //    for (const auto& value : row) {
        //        cout << value << " ";
        //    }
        //    cout << endl;
        //}
    }
    else {
        // 실패 시 에러 메시지 출력
        cerr << "Selection failed: " << PQerrorMessage(conn) << endl;

        PQclear(res);// 결과 클리어

        return listvector;
    }

    return listvector;
}

// 연결 확인 함수
bool AWS::RDSckeckConnection()
{
    // 연결 성공 여부 확인
    if (PQstatus(conn) != CONNECTION_OK) {
        // 연결 실패 시 에러 메시지 출력
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn); // 연결 종료
        return false;
    }
    else {
        // 연결 성공 시 메시지 출력
        cout << "Connected to database" << endl;
        return true;
    }
}

////////////////////////////////////////////////////////////
////////////////////S3//////////////////////////////////////
////////////////////////////////////////////////////////////

// 오브젝트 추가 함수
bool AWS::PutObject(const String& fileName,string date)
{
    S3Client s3_client(m_clientConfig);// S3 클라이언트 생성
    string select_query = "SELECT id FROM (SELECT * FROM thing ORDER BY id DESC LIMIT 1)";
    //  string select_query = "SELECT * FROM thing";

    PGresult* res1 = PQexec(conn, select_query.c_str());

    if (PQresultStatus(res1) == PGRES_TUPLES_OK) {

        // 오브젝트 추가 요청 생성
        PutObjectRequest request;
        request.SetBucket(bucketName);
        request.SetKey(String(PQgetvalue(res1, 0, 0)) + ".jpg");

        // 파일 입력 스트림 생성
        shared_ptr<IOStream> inputData =
            MakeShared<FStream>("SampleAllocationTag",
                fileName.c_str(),
                std::ios_base::in | std::ios_base::binary);

        // 파일 읽기 오류 처리
        if (!*inputData) {
            cerr << "Error unable to read file " << fileName << endl;
            return false;
        }

        // 요청에 파일 데이터 설정
        request.SetBody(inputData);

        // 오브젝트 추가 요청 전송
        PutObjectOutcome outcome =
            s3_client.PutObject(request);

        // 오브젝트 추가 결과 처리
        if (!outcome.IsSuccess()) {
            cerr << "Error: PutObject: " <<
                outcome.GetError().GetMessage() << endl;
            return false;
        }
        else {
            RDSinserts3Data(date);
            cout << "Added object '" << fileName << "' to bucket '"
                << bucketName << "'." << endl;
        }
        return outcome.IsSuccess();

    }
    return false;
}

// 오브젝트 가져오기 함수
bool AWS::GetObject(const String& objectKey)
{
    S3Client client(m_clientConfig);// S3 클라이언트 생성

    // 오브젝트 가져오기 요청 생성
    GetObjectRequest request;
    request.SetBucket(bucketName);
    request.SetKey(objectKey);

    // 오브젝트 가져오기 요청 전송
    GetObjectOutcome outcome =
        client.GetObject(request);

    // 오브젝트 가져오기 결과 처리
    if (!outcome.IsSuccess()) {
        const S3Error& err = outcome.GetError();
        cerr << "Error: GetObject: " <<
            err.GetExceptionName() << ": " << err.GetMessage() << endl;
        return false;
    }
    //else {
    //    ofstream outFile(objectKey.c_str(), ios::binary);// 파일 출력 스트림 생성
    //    if (!outFile)
    //    {
    //        cerr << "Failed to open file for writing" << endl;
    //        return false;
    //    }

    //    // 오브젝트 데이터 파일로 저장
    //    outFile << outcome.GetResult().GetBody().rdbuf();
    //    outFile.close();

    //    cout << "Successfully retrieved '" << objectKey << "' from '"
    //        << bucketName << "'." << endl;
    //}

    return outcome.IsSuccess();

}

// 오브젝트 삭제 함수
bool AWS::DeleteObjects(const vector<String>& objectKey)
{
    // S3 클라이언트 생성
    S3Client client(m_clientConfig);
    DeleteObjectsRequest request;

    // 삭제할 오브젝트 설정
    Aws::S3::Model::Delete deleteObject;
    for (const String& ObjectKey : objectKey)
    {
        deleteObject.AddObjects(ObjectIdentifier().WithKey(ObjectKey));
    }

    // 요청에 삭제할 오브젝트와 버킷 설정
    request.SetDelete(deleteObject);
    request.SetBucket(bucketName);

    // 삭제 요청 수행
    DeleteObjectsOutcome outcome =
        client.DeleteObjects(request);

    // 결과 처리
    if (!outcome.IsSuccess()) {
        // 실패 시 에러 메시지 출력
        auto err = outcome.GetError();
        std::cerr << "Error deleting objects. " <<
            err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
    }
    else {
        // 성공 시 성공 메시지 출력
        cout << "Successfully deleted the objects.";
        for (size_t i = 0; i < objectKey.size(); ++i)
        {
            cout << objectKey[i];
            if (i < objectKey.size() - 1)
            {
                std::cout << ", ";
            }
        }

        cout << " from bucket " << bucketName << "." << std::endl;
    }
    // 성공 여부 반환
    return outcome.IsSuccess();
}

////////////////////////////////////////////////////////////
////////////////////ALL/////////////////////////////////////
////////////////////////////////////////////////////////////

//한번에 모든 테이블 데이터 올리기
void AWS::Allinput(
    string date,
    const char* inputData,
    const String& fileName,
    const char* columnname,
    const char* tablename
)
{
    RDSinsertData(columnname, inputData, tablename);
    PutObject(fileName.c_str(),date);

}

void AWS::test(string testid)
{

    vector<AWSLIST> listvector;

    // JOIN 쿼리 생성
    string join_query = "SELECT a.id, a.color, a.faulty, b.date, b.url FROM thing AS a LEFT OUTER JOIN s3data AS b ON a.id = b.id ORDER BY id DESC";

    // 쿼리 실행
    PGresult* res = PQexec(conn, join_query.c_str());

    // 쿼리 실행 결과 확인
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);// 가져온 행의 수
        int columns = PQnfields(res); // 가져온 열의 수
        for (int i = 0; i < rows; ++i) {
            AWSLIST row; // 각 행을 저장할 벡터

            // 실제 가져온 각각의 열을 저장
            row.id = PQgetvalue(res, i, 0);

            if (row.id == testid)
            {
                row.color = PQgetvalue(res, i, 1);
                row.faulty = PQgetvalue(res, i, 2);
                if (row.faulty == "t") {
                    row.faulty = "Faulty";
                }
                else if (row.faulty == "f") {
                    row.faulty = "OK";
                }
                row.date = PQgetvalue(res, i, 3);
                row.url = PQgetvalue(res, i, 4);


                TRACE("%s\r\n", row.id.c_str());
                TRACE("%s\r\n", row.color.c_str());
                TRACE("%s\r\n", row.faulty.c_str());
                TRACE("%s\r\n", row.date.c_str());
                TRACE("%s\r\n", row.url.c_str());
                return;

            }
        }

        PQclear(res);// 결과 클리어
    }
    else {
        // 실패 시 에러 메시지 출력
        cerr << "Selection failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);// 결과 클리어
    }


}

void AWS::testinput(string date, const char* inputData, const String& fileName, const char* columnname , const char* tablename )
{
    RDSinsertData(columnname, inputData, tablename);
    PutObject(fileName.c_str(), date);
}

//한번에 모든 테이블의 원하는 행 데이터 지우기
void AWS::AlldeleteData(const char* deleteline, const char* columnname, const char* tablename, const char* tablename2)
{
    RDSdeleteData(columnname, deleteline, tablename2);
    RDSdeleteData(columnname, deleteline, tablename);
    DeleteObjects({ string(deleteline) + ".jpg" });
    //truncate table tableName restart identity//tabelName 테이블의 시퀀스를 자동으로 재시작하며 테이블 데이터를 모두 삭제한다
}

// S3 연결 종료 함수
void AWS::S3connectionEnd() {
    ShutdownAPI(options);
}

//// 연결 종료 함수
//void AWS::RDSconnectionEnd()
//{
//    // 연결 종료
//    PQfinish(conn);
//}
//
//// S3연결 함수
//bool AWS::JoinS3()
//{
//    m_clientConfig.region = "ap-northeast-2";
//
//    return true;
//}
