#include "ProductService.h"
#include "ProductFormData.h"
#include "Product.h"

ProductService::ProductService(IDatabase * database) : db(database) {

}

Product* createProduct(ProductFormData *productData) {
    auto product = new Product(productData->name);

    if (productData->name.empty())
    {
        return product;
    }

    if (productData->type.empty())
    {
        return product;
    }

    product->type = ("Unknown");

    if ("Eyeshadow" == (productData->type) || "Mascara" == (productData->type))
    {
        product->type = (productData->type);
        product->family = ProductFamily::EYES;
        if ("Eyeshadow" == (productData->type) && product->name.find("Queen") != std::string::npos)
        {
            product->range = (QUEEN);
        }
    }

    product->range = (BUDGET);
    if (productData->packagingRecyclable)
    {
        product->range = (PROFESSIONAL);
    }

    if ("Foundation" == (productData->type))
    {
        if (productData->suggestedPrice > 10)
        {
            product->range = (PROFESSIONAL);
        }
    }

    if ("Lipstick" == (productData->type))
    {
        product->type = (productData->type);
        product->family = (ProductFamily::LIPS);
        if (productData->suggestedPrice > 10)
        {
            product->range = (PROFESSIONAL);
        }

        if (productData->suggestedPrice > 20)
        {
            product->range = (QUEEN);
        }
    }

    if ("Lip gloss" == (productData->type))
    {
        product->type = (productData->type);
        product->family = (ProductFamily::LIPS);
        if (productData->suggestedPrice > 10)
        {
            product->range = (QUEEN);
        }
    }

    if ("Mascara" == (productData->type))
    {
        product->family = (ProductFamily::LASHES);
        if (productData->suggestedPrice > 15)
        {
            product->range = (PROFESSIONAL);
        }

        if (productData->suggestedPrice > 25 && productData->packagingRecyclable)
        {
            product->range = (QUEEN);
        }
    }

    product->weight = (productData->weight);

    if ("Blusher" == (productData->type) || "Foundation" == (productData->type))
    {
        product->type = (productData->type);
        product->family = (ProductFamily::SKIN);
    }

    return product;

}
Response validateProduct(const Product& product, float suggestedPrice, bool packagingRecyclable) {
    if (product.name.empty())
    {
        return Response(0, Response::MISSING_DATA_ERROR, "Missing Name");
    }

    if (product.type.empty())
    {
        return Response(0, Response::MISSING_DATA_ERROR, "Missing Type");
    }

    if ("Lipstick" == (product.type))
    {
        if (suggestedPrice > 20)
        {
            if (product.weight > 0 && product.weight < 10)
            {
                return Response(0, Response::FAILED_QUALITY_CHECK, "Error - failed quality check for Queen Range");
            }
        }
    }

    if ("Lip gloss" == (product.type))
    {
        if (suggestedPrice > 20)
        {
//            if (product.weight > 0 && product.weight < 10)
            if (product.weight > 20)
            {
                return Response(0, Response::FAILED_QUALITY_CHECK, "Error - failed quality check for Queen Range");
            }
        }
    }

    if (product.weight < 0)
    {
        return Response(0, Response::WEIGHT_ERROR, "Weight error");
    }

    if ("Blusher" == (product.type) && product.weight > 10)
    {
        return Response(0, Response::WEIGHT_ERROR, "Error - weight too high");
    }

    if (!packagingRecyclable && product.range == QUEEN)
    {
        return Response(0, Response::FAILED_QUALITY_CHECK, "Error - failed quality check for Queen Range");
    }

    if ("Unknown" == (product.type))
    {
        return Response(0, Response::FAILED_QUALITY_CHECK, "Unknown product type " + product.type);
    }

    return Response(0, Response::OK, "Product Successfully Added");
}

Response ProductService::validateAndAdd(ProductFormData *productData) {
    auto product = createProduct(productData);
    auto response = validateProduct(*product, productData->suggestedPrice, productData->packagingRecyclable);
    if ( response.statusCode == Response::OK)
    {
        response.productId = db->storeProduct(product);
    }

    return response;
}
