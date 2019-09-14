#ifndef VALIDATE_AND_ADD_PRODUCT_REFACTORING_KATA_RESPONSE_H
#define VALIDATE_AND_ADD_PRODUCT_REFACTORING_KATA_RESPONSE_H

#include <string>

class Response {
public:
    enum StatusCode
    {
        WEIGHT_ERROR = -3,
        MISSING_DATA_ERROR = -2,
        FAILED_QUALITY_CHECK = -1,
        OK = 0
    };
    Response(int productId, StatusCode statusCode, std::string message);
    int productId;
    StatusCode statusCode;
    std::string message;

    std::string to_string();
};


#endif //VALIDATE_AND_ADD_PRODUCT_REFACTORING_KATA_RESPONSE_H
