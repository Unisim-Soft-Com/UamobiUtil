#pragma once
#include <QHash>
#include <QVector>
#include <QStringList>

/*
	This file is containing templating mechanism for extern query binding. Main purpose of this
	file is to avoid hardcoded default queries. It performs binding via hash, extracted from ini file.
	Also this file contains OverloadableQuery class - main tool for overloading queries through 
	mode constructor. It tracks provided arguments and required arguments.

*/
namespace QueryTemplates
{
	// This enum is used for enhancing performance of query binding
	enum QueryId
	{
		ping,						// no arguments required 
		userList,					// 1 arguments required 
		Login,						// 3 arguments required 
		LogOut,						// 0 arguments required 
		modeList,					// 1 arguments required 
		selectMode,					// 2 arguments required 
		placeList,					// 0 arguments required 
		selectPlace,				// 1 arguments required 
		lockDocument,				// 1 arguments required 
		unlockDocument,				// 1 arguments required 
		documentGetAllowedTypes,	// 0 arguments required 
		documentSetFilter,			// 1 arguments required 
		documentGetAllowedToCreate,	// 0 arguments required 
		documentNewParent,			// 1 arguments required 
		documentSearchItems,		// 2 arguments required 
		getItemInfo,				// 2 arguments required 
		documentGetResults,			// 1 arguments required 
		documentResultGetBox,		// 1 arguments required 
		docResultEditByBoxId,		// 4 arguments required 
		docResultDeleteItem,		// 1 arguments required 
		docUpdateParameters,		// 4 arguments required 
		docListSubdocuments,		// 0 arguments required 
		inventoryListParentDocs,	// 0 arguments required 
		listPhotoItems,				// 0 arguments required 
		inventoryNewDocument,		// 3 arguments required 
		inventoryAddItem,			// 3 arguments required 
		invAddItemExpDate,			// 2 arguments required 
		receiptListSuppliers,		// 2 arguments required 
		warehouseList,				// 2 arguments required
		receiptListOrders,			// 2 arguments required 
		receiptGetOrderInfo,		// 2 arguments required 
		receiptNewDocument,			// 3 arguments required 
		receiptAddItem,				// 2 arguments required 
		receiptAddItemExpDate,		// 2 arguments required 
		getFilterItem,				// 1 arguments required 
		documentDeleteAll,			// 0 arguments required 
		docDeleteByBarcode,			// 2 arguments required 
		recListTemplated,			// 1 arguments required 
		applyBarcodeFilter,			// 2 arguments required 
		receiptAddItemExpanded,		// 3 arguments required 
		docGetItemLabel,
		setVersionForBarcode,
		receiptOrderByBC,
		addIdToParentDocs,
		deleteInvoiceById,
		editInvoiceById,
		editItemById,
		getShopLabel				// 2 arguments required
	};

	// this function is used for initializing cache in old compilers
	QHash<QueryId, QString> _initCache();
	// this function performs binding to quantity of arguments. Deprecated because of overloadable query
	int getQueryArguments(const QueryId);
	// performs argument quantity check to correspond required
	bool assertArgQuantity(int argc, QueryId id);
	// this cache contains bound queries
	extern QHash<QueryId, QString> queryCache;

	class OverloadableQuery
		// wraps query and it's arguments to allow overloading
	{
	private:
		// this id is used for extracting query url from cache if no url provided
		QueryId expectedQuery;
		// this url is used as template for inserting arguments
		QString overloadedQuery;
		// this variable contains maximum arguments this query can handle and uses it's negative values as flags
		int argumentListLength;
		// this vector holds positioning of arguments, which can differ from provided list's args
		QVector<int> emplaceValues;
		// this vector holds list of names required to insert into query. Debug use only
		QStringList valuesForUnpacking;
		
		// obtains argument index from valuesForUnpacking
		inline int _argNameToInt(const QString arg) const;
		// finds query to use in templating - or from cache, or from url
		inline const QString& assertRightQuery() const;
	public:
		OverloadableQuery();
		explicit OverloadableQuery(QueryId id);
		explicit OverloadableQuery(QString oq);
		explicit OverloadableQuery(int argListLength, QueryId queryId);
		explicit OverloadableQuery(QStringList allValues, QStringList unpackedValues);
		OverloadableQuery(QueryId id, QStringList allValues, QStringList unpackedValues);
		OverloadableQuery(QString oq, QStringList allValues, 
			QStringList unpackedValues);
		OverloadableQuery(QString oq, QStringList unpacked);
		OverloadableQuery(QStringRef oq, QVector<QStringRef>& unpacked);

		enum useFlags { DefaultQ = -2, NullQ = -4 };

		// post-constructor can replace query url 
		void setExpected(QueryId);
		// uses default id and argument lists to prepare query for work
		void assertAndSetMapping(QueryId defId, QStringList allArgs, QStringList defMap);
		// returns copy of this with applied assertAndSetMapping
		OverloadableQuery assertedAndMappedCopy(QueryId defId, QStringList allArgs = QStringList(), QStringList defMap = QStringList()) const;
		// post constructor which used in case if query has no mapping or wrong one
		void setFieldMapping(QStringList normal, QStringList unpacked);
		// post constructor uses all arguments list to reset
		void setAllArgsFilter(QStringList allArgs);
		// checks if this query is mapped for this quantity of arguments
		bool assertArgQuantity(int argc) const;
		// default flag is -2 in argumentListLength
		bool isDefault() const;
		bool isNull() const;
		// true if mapping was not finished
		bool requiresAllArgs() const;
		// true if only url provided
		bool requiresRemapping() const { 
			return !overloadedQuery.isEmpty() && argumentListLength == 0 && valuesForUnpacking.isEmpty();
		}
		// returns query id
        QueryId getQID() const { return expectedQuery; }
		// returns static default query which can be used as null
		static const OverloadableQuery& defaultQuery();
		static const OverloadableQuery& nullQuery();
		// main interface function, applies arguments and returns url ready to be sent
		QString filterAndApply(QStringList arguments, QString ssid) const;
	};
	// typedef for local caches
	typedef QHash<QueryTemplates::QueryId, QueryTemplates::OverloadableQuery> QueryCache;
}
