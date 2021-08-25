#include "QueryTemplates.h"
#include <QtCore/QSettings>
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#ifndef QStringLiteral
#define QStringLiteral(A) QString::fromUtf8("" A "" , sizeof(A)-1)
#endif
namespace QueryTemplates
{
	QHash<QueryId, QString> queryCache(_initCache());

    QHash<QueryId, QString> _initCache()
	{
		QHash<QueryId, QString>c;
		QSettings queryTemplates("querytemplates.ini", QSettings::IniFormat);
		bool ok = !(queryTemplates.value("ping").toString().isEmpty());
		c[ping] = queryTemplates.value(QStringLiteral("ping"), QStringLiteral("ping")).toString();
		c[userList] = queryTemplates.value(QStringLiteral("userList"), QStringLiteral("list_users%1")).toString();
		c[Login] = queryTemplates.value(QStringLiteral("Login"), QStringLiteral("log_in&username=%1&password=%2&version=%3")).toString();
		c[LogOut] = queryTemplates.value(QStringLiteral("LogOut"), QStringLiteral("log_out&session=%1")).toString();
		c[modeList] = queryTemplates.value(QStringLiteral("modeList"), QStringLiteral("list_modes&session=%1&lang=%2")).toString();
		c[selectMode] = queryTemplates.value(QStringLiteral("selectMode"), QStringLiteral("select_mode&session=%1&mode=%2&submode=%3")).toString();
		c[placeList] = queryTemplates.value(QStringLiteral("placeList"), QStringLiteral("list_places&session=%1")).toString();
		c[selectPlace] = queryTemplates.value(QStringLiteral("selectPlace"), QStringLiteral("select_place&session=%1&place=%2")).toString();
		c[lockDocument] = queryTemplates.value(QStringLiteral("lockDocument"), QStringLiteral("doc_edit&session=%1&document=%2")).toString();
		c[unlockDocument] = queryTemplates.value(QStringLiteral("unlockDocument"), QStringLiteral("doc_exit&session=%1&save=%2")).toString();
		c[documentGetAllowedTypes] = queryTemplates.value(QStringLiteral("documentGetAllowedTypes"), QStringLiteral("doc_get_allowed_types&session=%1")).toString();
		c[documentSetFilter] = queryTemplates.value(QStringLiteral("documentSetFilter"), QStringLiteral("doc_set_filter&session=%1&filter=%2")).toString();
		c[documentGetAllowedToCreate] = queryTemplates.value(QStringLiteral("documentGetAllowedToCreate"), QStringLiteral("doc_get_allowed_to_create_types&session=%1")).toString();
		c[documentNewParent] = queryTemplates.value(QStringLiteral("documentNewParent"), QStringLiteral("doc_parent_new&session=%1&sysfid=%2")).toString();
		c[documentSearchItems] = queryTemplates.value(QStringLiteral("documentSearchItems"), QStringLiteral("doc_search_items&session=%1&text=%2&page=%3")).toString();
		c[getItemInfo] = queryTemplates.value(QStringLiteral("getItemInfo"), QStringLiteral("doc_get_item_info&session=%1&barcode=%2&printer=%3")).toString();
		c[documentGetResults] = queryTemplates.value(QStringLiteral("documentGetResults"), QStringLiteral("doc_list_items&session=%1%2%3")).toString();
		c[documentResultGetBox] = queryTemplates.value(QStringLiteral("documentResultsGetBox"), QStringLiteral("doc_result_get_box_data&session=%1&box_id=%2")).toString();
		c[docResultEditByBoxId] = queryTemplates.value(QStringLiteral("docResultEditByBoxId"), QStringLiteral("doc_result_edit_by_box_id&session=%1&box_id=%2&qty=%3&qty_of_pkg=%4&lot=%5")).toString();
		c[docResultDeleteItem] = queryTemplates.value(QStringLiteral("docResultDeleteItem"), QStringLiteral("doc_result_delete_item&session=%1&box_id=%2")).toString();
		c[docUpdateParameters] = queryTemplates.value(QStringLiteral("docUpdateParameters"), QStringLiteral("doc_update_params&session=%1&document=%2&cancelled=%3&closed=%4&comment=%5")).toString();
		c[docListSubdocuments] = queryTemplates.value(QStringLiteral("docListSubdocuments"), QStringLiteral("doc_list_subdocuments&session=%1")).toString();
		c[inventoryListParentDocs] = queryTemplates.value(QStringLiteral("inventoryListParentDocs"), QStringLiteral("inv_list_parent_docs&session=%1")).toString();
		c[listPhotoItems] = queryTemplates.value(QStringLiteral("listPhotoItems"), QStringLiteral("get_list_photo_items&session=%1")).toString();
		c[inventoryNewDocument] = queryTemplates.value(QStringLiteral("inventoryNewDoc"), QStringLiteral("inv_new_doc&session=%1&date=%2&parent=%3&comment=%4")).toString();
		c[inventoryAddItem] = queryTemplates.value(QStringLiteral("inventoryAddItem"), QStringLiteral("inv_add_item&session=%1&code=%2&qty=%3&show_code=%4")).toString();
		c[invAddItemExpDate] = queryTemplates.value(QStringLiteral("invAddItemExpDate"), QStringLiteral("inv_add_item_w_ed&session=%1&code=%2&expdates=%3")).toString();
		c[receiptListSuppliers] = queryTemplates.value(QStringLiteral("receiptListSuppliers"), QStringLiteral("rec_list_suppliers&session=%1&text=%2&hasorders=%3")).toString();
		c[warehouseList] = queryTemplates.value(QStringLiteral("warehouseList"), QStringLiteral("rec_list_warehouses&session=%1&text=%2&hasorders=%3")).toString();
		c[receiptListOrders] = queryTemplates.value(QStringLiteral("receiptListOrders"), QStringLiteral("rec_list_orders&session=%1&%2%3")).toString();
		c[receiptGetOrderInfo] = queryTemplates.value(QStringLiteral("receiptGetOrderInfo"), QStringLiteral("rec_get_order_info&session=%1&order=%2&supplier=%3")).toString();
		c[receiptNewDocument] = queryTemplates.value(QStringLiteral("receiptNewDocument"), QStringLiteral("rec_new_doc&session=%1&date=%2&parent=%3&comment=%4")).toString();
		c[receiptAddItem] = queryTemplates.value(QStringLiteral("receiptAddItem"), QStringLiteral("rec_add_item&session=%1&barcode=%2&qty=%3")).toString();
		c[receiptAddItemExpDate] = queryTemplates.value(QStringLiteral("receiptAddItemExpDate"), QStringLiteral("rec_add_item_w_ed&session=%1&code=%2&expdates=%3")).toString();
		c[getFilterItem] = queryTemplates.value(QStringLiteral("getFilterItem"), QStringLiteral("get_filter_item&session=%1&parent=%2")).toString();
		c[documentDeleteAll] = queryTemplates.value(QStringLiteral("DocumentDeleteAll"), QStringLiteral("doc_result_delete_all&session=%1")).toString();
		c[docDeleteByBarcode] = queryTemplates.value(QStringLiteral("docDeleteByBarcode"), QStringLiteral("doc_result_delete_by_bc&session=%1&barcode=%2%3")).toString();
        c[recListTemplated] = queryTemplates.value(QStringLiteral("recListTemplated"), QStringLiteral("rec_list_%2&session=%1&text=%3%4")).toString();
		c[applyBarcodeFilter] = queryTemplates.value(QStringLiteral("applyBarcodeFilter"), QStringLiteral("select_filter_item&session=%1&filter=%2&value=%3")).toString();
		c[receiptAddItemExpanded] = queryTemplates.value(QStringLiteral("receiptAddItemExpanded"), QStringLiteral("rec_add_item&session=%1&barcode=%2&qty=%3&price=0&prs=%4")).toString();
		c[docGetItemLabel] = queryTemplates.value(QStringLiteral("docGetItemLabel"), QStringLiteral("doc_get_item_label&session=%1&barcode=%2&qty=%3&printer=%4")).toString();
		c[setVersionForBarcode] = queryTemplates.value(QStringLiteral("setVersionForBarcode"), QStringLiteral("set_version_for_barcode&session=%1&barcode=%2&version=%3")).toString();
		c[receiptOrderByBC] = queryTemplates.value(QStringLiteral("receiptOrderByBC"), QStringLiteral("rec_get_order_by_barcode&session=%1&barcode=%2")).toString();
		c[addIdToParentDocs] = queryTemplates.value(QStringLiteral("addIdToParentDocs"), QStringLiteral("rec_add_order_doc&session=%1&nrdoc=%2&parents=%3")).toString();
		c[deleteInvoiceById] = queryTemplates.value(QStringLiteral("deleteInvoiceById"), QStringLiteral("doc_delete_invoice&session=%1&nn=%2")).toString();
		c[editInvoiceById] = queryTemplates.value(QStringLiteral("editInvoiceById"), QStringLiteral("rec_edit_invoice&session=%1&old_value=%2%3")).toString();
		c[editItemById] = queryTemplates.value(QStringLiteral("editItemById"), QStringLiteral("doc_result_delete_by_bc&session=%1&barcode=%2%3")).toString();
		c[getShopLabel] = queryTemplates.value(QStringLiteral("getShopLabel"), QStringLiteral("print_department_label&session=%1&qty=%2&printer=%3")).toString();
		if (!ok)
		{
			int i = 0;
			queryTemplates.setValue(QStringLiteral("ping"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("userList"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("Login"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("LogOut"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("modeList"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("selectMode"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("placeList"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("selectPlace"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("lockDocument"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("unlockDocument"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("documentGetAllowedTypes"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("documentSetFilter"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("documentGetAllowedToCreate"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("documentNewParent"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("documentSearchItems"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("getItemInfo"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("documentGetResults"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("documentResultsGetBox"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("docResultEditByBoxId"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("docResultDeleteItem"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("docUpdateParameters"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("docListSubdocuments"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("inventoryListParentDocs"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("listPhotoItems"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("inventoryNewDoc"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("inventoryAddItem"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("invAddItemExpDate"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("receiptListSuppliers"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("warehouseList"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("receiptListOrders"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("receiptGetOrderInfo"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("receiptNewDocument"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("receiptAddItem"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("receiptAddItemExpDate"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("getFilterItem"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("DocumentDeleteAll"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("docDeleteByBarcode"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("recListTemplated"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("applyBarcodeFilter"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("receiptAddItemExpanded"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("docGetItemLabel"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("setVersionForBarcode"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("receiptOrderByBC"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("addIdToParentDocs"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("deleteInvoiceById"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("editInvoiceById"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("editItemById"), c[static_cast<QueryId>(i++)]);
			queryTemplates.setValue(QStringLiteral("getShopLabel"), c[static_cast<QueryId>(i++)]);
		}
		return c;
	}


	OverloadableQuery nullquery(OverloadableQuery::NullQ, ping);
	OverloadableQuery defQuery(OverloadableQuery::DefaultQ, ping);

int getQueryArguments(const QueryId id)
{
	switch (id)
	{
	case ping:
		return   0;
	case userList:
		return   1;
	case Login:
		return   2;
	case LogOut:
		return   0;
	case modeList:
		return   1;
	case selectMode:
		return   2;
	case placeList:
		return   0;
	case selectPlace:
		return   1;
	case lockDocument:
		return   1;
	case unlockDocument:
		return   1;
	case documentGetAllowedTypes:
		return   0;
	case documentSetFilter:
		return   1;
	case documentGetAllowedToCreate:
		return   0;
	case documentNewParent:
		return   1;
	case documentSearchItems:
		return   2;
	case getItemInfo:
		return   2;
	case documentGetResults:
		return   2;
	case documentResultGetBox:
		return   1;
	case docResultEditByBoxId:
		return   4;
	case docResultDeleteItem:
		return   1;
	case docUpdateParameters:
		return   4;
	case docListSubdocuments:
		return   0;
	case inventoryListParentDocs:
		return   0;
	case listPhotoItems:
		return   0;
	case inventoryNewDocument:
		return   3;
	case inventoryAddItem:
		return   3;
	case invAddItemExpDate:
		return   2;
	case receiptListSuppliers:
		return   2;
	case receiptListOrders:
		return   2;
	case receiptGetOrderInfo:
		return   2;
	case receiptNewDocument:
		return   3;
	case receiptAddItem:
		return   2;
	case receiptAddItemExpDate:
		return   2;
	case getFilterItem:
		return   1;
	case documentDeleteAll:
		return   0;
	case docDeleteByBarcode:
		return   2;
	case recListTemplated:
		return   2;
	case applyBarcodeFilter:
		return   2;
	case receiptAddItemExpanded:
		return   3;
	case docGetItemLabel:
		return   3;
	case setVersionForBarcode:
		return 2;
	case receiptOrderByBC:
		return 1;
	case addIdToParentDocs:
		return 2;
	case deleteInvoiceById:
		return 1;
	case editInvoiceById:
		return 2;
	case editItemById:
		return 2;
	case getShopLabel:
		return 2;
	default:
		return 0;
	}
}
bool assertArgQuantity(int argc, QueryTemplates::QueryId id)
{
	using namespace QueryTemplates;
#ifdef DEBUG
	if (!(getQueryArguments(id)== argc))
		detrace_NETERROR("error sending query with a/a: ", argc << " / " << getQueryArguments(id));
#endif
	return argc == getQueryArguments(id);
}

int QueryTemplates::OverloadableQuery::_argNameToInt(const QString arg) const
{
	return valuesForUnpacking.indexOf(arg);
}

const QString& QueryTemplates::OverloadableQuery::assertRightQuery() const
{
	if (overloadedQuery.isEmpty())
		return queryCache[expectedQuery];
	else
		return overloadedQuery;
}

OverloadableQuery::OverloadableQuery()
	: expectedQuery(ping), overloadedQuery(),
	argumentListLength(-2), emplaceValues(), valuesForUnpacking()
{
}

QueryTemplates::OverloadableQuery::OverloadableQuery(QueryId id)
	: expectedQuery(id), overloadedQuery(), argumentListLength(),
	emplaceValues(), valuesForUnpacking()
{
}

QueryTemplates::OverloadableQuery::OverloadableQuery(QString oq)
    :expectedQuery(ping), overloadedQuery(oq), argumentListLength(), emplaceValues(), valuesForUnpacking()
{
}

QueryTemplates::OverloadableQuery::OverloadableQuery(int argListLength, QueryId queryId)
	: expectedQuery(queryId), overloadedQuery(), argumentListLength(argListLength), emplaceValues()
{
}

QueryTemplates::OverloadableQuery::OverloadableQuery(QStringList allValues, QStringList unpackedValues)
	: expectedQuery(), overloadedQuery(), argumentListLength(allValues.count()), emplaceValues(), valuesForUnpacking(unpackedValues)
{
	for (int i = 0;
		i < ((unpackedValues.count() <= argumentListLength)? unpackedValues.count() : argumentListLength);
		++i)
	{
		int argIndex = allValues.indexOf(unpackedValues.at(i));
		if (argIndex == -1)
			continue;
		emplaceValues.push_back(
			argIndex
		);
	}
}

QueryTemplates::OverloadableQuery::OverloadableQuery(QueryId id, QStringList allValues,
	QStringList unpackedValues)
	: expectedQuery(id), overloadedQuery(), argumentListLength(allValues.count()), 
	emplaceValues(), valuesForUnpacking(unpackedValues)
{
	for (int i = 0; 
		i < unpackedValues.count(); 
		++i)
	{
		int argIndex = allValues.indexOf(unpackedValues.at(i));
		if (argIndex == -1)
			continue;
		emplaceValues.push_back(
			argIndex
		);
	}
}

QueryTemplates::OverloadableQuery::OverloadableQuery(QString oq, QStringList allValues, QStringList unpackedValues)
	: expectedQuery(), overloadedQuery(oq), argumentListLength(allValues.count()),
	emplaceValues(), valuesForUnpacking(unpackedValues)
{
	for (int i = 0;
		i < ((unpackedValues.count() <= argumentListLength) ? unpackedValues.count() : argumentListLength);
		++i)
	{
		int argIndex = allValues.indexOf(unpackedValues.at(i));
		if (argIndex == -1)
			continue;
		emplaceValues.push_back(
			argIndex
		);
	}
}

QueryTemplates::OverloadableQuery::OverloadableQuery(QString oq, QStringList unpacked)
	:expectedQuery(), overloadedQuery(oq), argumentListLength(0), emplaceValues(), 
	valuesForUnpacking(unpacked)
{
}

QueryTemplates::OverloadableQuery::OverloadableQuery(QStringRef oq, QVector<QStringRef>& unpacked)
	:expectedQuery(), overloadedQuery(oq.toString()), argumentListLength(0), emplaceValues(),
	valuesForUnpacking()
{
	QVector<QStringRef>::iterator begin = unpacked.begin();
	valuesForUnpacking.reserve(unpacked.count());
	while (begin != unpacked.end())
	{
		valuesForUnpacking.push_back((begin++)->toString());
	}
}




void QueryTemplates::OverloadableQuery::setExpected(QueryId exp)
{
	expectedQuery = exp;
}

void QueryTemplates::OverloadableQuery::assertAndSetMapping(QueryId defId, QStringList normal, QStringList defMap)
{
	if (isDefault())
	{
		expectedQuery = defId;
		setFieldMapping(normal, defMap);
	}
	else if (requiresAllArgs())
	{
		setAllArgsFilter(normal);
	}
	else
	{ 
		setFieldMapping(normal, defMap);
	}
}

OverloadableQuery OverloadableQuery::assertedAndMappedCopy(QueryId defId, QStringList allArgs, QStringList defMap) const
{
	OverloadableQuery oq(*this);
	if ((allArgs.isEmpty() && defMap.isEmpty() )|| oq.isNull())
		return oq;
	oq.assertAndSetMapping(defId, allArgs, defMap);
	return oq;
}

void QueryTemplates::OverloadableQuery::setFieldMapping(QStringList normal, QStringList unpacked)
{
	valuesForUnpacking = unpacked;
	argumentListLength = normal.count();
	emplaceValues.clear();
	for (int i = 0;
		i < ((unpacked.count() <= argumentListLength) ? unpacked.count() : argumentListLength);
		++i)
	{
		int argIndex = normal.indexOf(unpacked.at(i));
		if (argIndex == -1)
			continue;
		emplaceValues.push_back(
			argIndex
		);
	}
}

void QueryTemplates::OverloadableQuery::setAllArgsFilter(QStringList allArgs)
{
	argumentListLength = allArgs.count();
	for (int i = 0;
		i < ((valuesForUnpacking.count() <= argumentListLength) ? valuesForUnpacking.count() : argumentListLength);
		++i)
	{
		int argIndex = allArgs.indexOf(valuesForUnpacking.at(i));
		if (argIndex == -1)
			continue;
		emplaceValues.push_back(
			argIndex
		);
	}
}

bool QueryTemplates::OverloadableQuery::assertArgQuantity(int argc) const
{
#ifdef DEBUG
	if (!(argumentListLength == argc))
		detrace_NETERROR("error sending query with a/a: ", argc << " / " << argumentListLength);
#endif
	return argumentListLength == argc;
}

bool QueryTemplates::OverloadableQuery::isDefault() const
{
	return argumentListLength == DefaultQ;
}

bool OverloadableQuery::isNull() const
{
	return argumentListLength  == NullQ;
}

bool QueryTemplates::OverloadableQuery::requiresAllArgs() const
{
	return argumentListLength == 0 && (
		(valuesForUnpacking.count() > 0 && 
			(!overloadedQuery.isEmpty() || expectedQuery != ping)));
}

const QueryTemplates::OverloadableQuery& QueryTemplates::OverloadableQuery::defaultQuery()
{
	return defQuery;
}

const OverloadableQuery& OverloadableQuery::nullQuery()
{
	return nullquery;
}

QString QueryTemplates::OverloadableQuery::filterAndApply(QStringList arguments, QString ssid) const
{
	QVector<int>::const_iterator begin = emplaceValues.constBegin();
	QString result = assertRightQuery().arg(ssid);
	if (arguments.count() < argumentListLength)
	{
		int borderValue = arguments.count();
		while (begin != emplaceValues.end())
		{
			if (*begin <= borderValue)
			{
				result = result.arg(arguments.at(*begin));
			}
			++begin;
		}
	}
	else
	{
		while (begin != emplaceValues.end())
		{
			result = result.arg(arguments.at(*begin));
			++begin;
		}
	}
	return result;
}


}
