#include <ProductFormData.h>
#include <ProductService.h>
#include "doctest.h"
#include "ApprovalTests.hpp"
#include "FakeDatabase.h"
#include <string>


string doValidateAndAdd(const string &name, const string &type, float weight, float price, bool packagingRecyclable);

TEST_CASE ("test_validate_and_add") {
    std::vector<std::string> names{"", "Sample product", "Sample Queen Product"};
    std::vector<std::string> types{"", "Lipstick", "Eyeshadow", "Mascara", "Blusher", "Foundation", "Unknown"};
    std::vector<float> weights{-1, 5, 11};
    std::vector<float> prices{10, 11, 21, 26};
    std::vector<bool> recyclables{false, true};

    CombinationApprovals::verifyAllCombinations(doValidateAndAdd,
                                                names,
                                                types,
                                                weights,
                                                prices,
                                                recyclables);
}

string doValidateAndAdd(const string &name, const string &type, float weight, float price, bool packagingRecyclable) {
    ProductFormData productData(name, type, weight, price, packagingRecyclable);
    FakeDatabase db{};
    ProductService sut(&db);

    Response response = sut.validateAndAdd(&productData);

    std::string responseAndProduct;
    responseAndProduct.append(response.to_string());
    if (db.product_) {
        responseAndProduct.append(" ");
        responseAndProduct.append(db.product_->to_string());
    }
    return responseAndProduct;
}
