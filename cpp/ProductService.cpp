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
            product->range = (ProductRange::QUEEN);
        }
    }

    product->range = (ProductRange::BUDGET);
    if (productData->packagingRecyclable)
    {
        product->range = (ProductRange::PROFESSIONAL);
    }

    if ("Foundation" == (productData->type))
    {
        if (productData->suggestedPrice > 10)
        {
            product->range = (ProductRange::PROFESSIONAL);
        }
    }

    if ("Lipstick" == (productData->type))
    {
        product->type = (productData->type);
        product->family = (ProductFamily::LIPS);
        if (productData->suggestedPrice > 10)
        {
            product->range = (ProductRange::PROFESSIONAL);
        }

        if (productData->suggestedPrice > 20)
        {
            product->range = (ProductRange::QUEEN);
        }
    }

    // The reason for the refactoring is that you need to add a new type of product - Lip gloss.
    // It should behave the same as the 'Lipstick' type with a small difference -
    // if the price is greater than 10 it should be put in the 'Queen' range instead of the 'Professional' range.
    // If the weight is over 20 then you should return an error.
    if ("Lip gloss" == (productData->type))
    {
        product->type = (productData->type);
        product->family = (ProductFamily::LIPS);
        if (productData->suggestedPrice > 10)
        {
            product->range = (ProductRange::QUEEN);
        }

        if (productData->suggestedPrice > 20)
        {
            product->range = (ProductRange::QUEEN);
        }
    }

    if ("Mascara" == (productData->type))
    {
        product->family = (ProductFamily::LASHES);
        if (productData->suggestedPrice > 15)
        {
            product->range = (ProductRange::PROFESSIONAL);
        }

        if (productData->suggestedPrice > 25 && productData->packagingRecyclable)
        {
            product->range = (ProductRange::QUEEN);
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
Response validate(const Product& product, float suggestedPrice, bool packagingRecyclable) {
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

    // The reason for the refactoring is that you need to add a new type of product - Lip gloss.
    // It should behave the same as the 'Lipstick' type with a small difference -
    // if the price is greater than 10 it should be put in the 'Queen' range instead of the 'Professional' range.
    // If the weight is over 20 then you should return an error.
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

    if (!packagingRecyclable && product.range == ProductRange::QUEEN)
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
    auto response = validate(*product, productData->suggestedPrice, productData->packagingRecyclable);
    if ( response.statusCode == Response::OK)
    {
        response.productId = db->storeProduct(product);
    }

    return response;
}
