#include "librpc/gen_ndr/ndr_lsa.h"
#ifndef __CLI_LSARPC__
#define __CLI_LSARPC__
NTSTATUS rpccli_lsa_Close(struct rpc_pipe_client *cli,
			  TALLOC_CTX *mem_ctx,
			  struct policy_handle *handle);
NTSTATUS rpccli_lsa_Delete(struct rpc_pipe_client *cli,
			   TALLOC_CTX *mem_ctx,
			   struct policy_handle *handle);
NTSTATUS rpccli_lsa_EnumPrivs(struct rpc_pipe_client *cli,
			      TALLOC_CTX *mem_ctx,
			      struct policy_handle *handle,
			      uint32_t *resume_handle,
			      struct lsa_PrivArray *privs,
			      uint32_t max_count);
NTSTATUS rpccli_lsa_QuerySecurity(struct rpc_pipe_client *cli,
				  TALLOC_CTX *mem_ctx,
				  struct policy_handle *handle,
				  uint32_t sec_info,
				  struct sec_desc_buf **sdbuf);
NTSTATUS rpccli_lsa_SetSecObj(struct rpc_pipe_client *cli,
			      TALLOC_CTX *mem_ctx,
			      struct policy_handle *handle,
			      uint32_t sec_info,
			      struct sec_desc_buf *sdbuf);
NTSTATUS rpccli_lsa_ChangePassword(struct rpc_pipe_client *cli,
				   TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_OpenPolicy(struct rpc_pipe_client *cli,
			       TALLOC_CTX *mem_ctx,
			       uint16_t *system_name,
			       struct lsa_ObjectAttribute *attr,
			       uint32_t access_mask,
			       struct policy_handle *handle);
NTSTATUS rpccli_lsa_QueryInfoPolicy(struct rpc_pipe_client *cli,
				    TALLOC_CTX *mem_ctx,
				    struct policy_handle *handle,
				    enum lsa_PolicyInfo level,
				    union lsa_PolicyInformation **info);
NTSTATUS rpccli_lsa_SetInfoPolicy(struct rpc_pipe_client *cli,
				  TALLOC_CTX *mem_ctx,
				  struct policy_handle *handle,
				  enum lsa_PolicyInfo level,
				  union lsa_PolicyInformation *info);
NTSTATUS rpccli_lsa_ClearAuditLog(struct rpc_pipe_client *cli,
				  TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_CreateAccount(struct rpc_pipe_client *cli,
				  TALLOC_CTX *mem_ctx,
				  struct policy_handle *handle,
				  struct dom_sid2 *sid,
				  uint32_t access_mask,
				  struct policy_handle *acct_handle);
NTSTATUS rpccli_lsa_EnumAccounts(struct rpc_pipe_client *cli,
				 TALLOC_CTX *mem_ctx,
				 struct policy_handle *handle,
				 uint32_t *resume_handle,
				 struct lsa_SidArray *sids,
				 uint32_t num_entries);
NTSTATUS rpccli_lsa_CreateTrustedDomain(struct rpc_pipe_client *cli,
					TALLOC_CTX *mem_ctx,
					struct policy_handle *handle,
					struct lsa_DomainInfo *info,
					uint32_t access_mask,
					struct policy_handle *trustdom_handle);
NTSTATUS rpccli_lsa_EnumTrustDom(struct rpc_pipe_client *cli,
				 TALLOC_CTX *mem_ctx,
				 struct policy_handle *handle,
				 uint32_t *resume_handle,
				 struct lsa_DomainList *domains,
				 uint32_t max_size);
NTSTATUS rpccli_lsa_LookupNames(struct rpc_pipe_client *cli,
				TALLOC_CTX *mem_ctx,
				struct policy_handle *handle,
				uint32_t num_names,
				struct lsa_String *names,
				struct lsa_RefDomainList **domains,
				struct lsa_TransSidArray *sids,
				enum lsa_LookupNamesLevel level,
				uint32_t *count);
NTSTATUS rpccli_lsa_LookupSids(struct rpc_pipe_client *cli,
			       TALLOC_CTX *mem_ctx,
			       struct policy_handle *handle,
			       struct lsa_SidArray *sids,
			       struct lsa_RefDomainList **domains,
			       struct lsa_TransNameArray *names,
			       uint16_t level,
			       uint32_t *count);
NTSTATUS rpccli_lsa_CreateSecret(struct rpc_pipe_client *cli,
				 TALLOC_CTX *mem_ctx,
				 struct policy_handle *handle,
				 struct lsa_String name,
				 uint32_t access_mask,
				 struct policy_handle *sec_handle);
NTSTATUS rpccli_lsa_OpenAccount(struct rpc_pipe_client *cli,
				TALLOC_CTX *mem_ctx,
				struct policy_handle *handle,
				struct dom_sid2 *sid,
				uint32_t access_mask,
				struct policy_handle *acct_handle);
NTSTATUS rpccli_lsa_EnumPrivsAccount(struct rpc_pipe_client *cli,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle *handle,
				     struct lsa_PrivilegeSet **privs);
NTSTATUS rpccli_lsa_AddPrivilegesToAccount(struct rpc_pipe_client *cli,
					   TALLOC_CTX *mem_ctx,
					   struct policy_handle *handle,
					   struct lsa_PrivilegeSet *privs);
NTSTATUS rpccli_lsa_RemovePrivilegesFromAccount(struct rpc_pipe_client *cli,
						TALLOC_CTX *mem_ctx,
						struct policy_handle *handle,
						uint8_t remove_all,
						struct lsa_PrivilegeSet *privs);
NTSTATUS rpccli_lsa_GetQuotasForAccount(struct rpc_pipe_client *cli,
					TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_SetQuotasForAccount(struct rpc_pipe_client *cli,
					TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_GetSystemAccessAccount(struct rpc_pipe_client *cli,
					   TALLOC_CTX *mem_ctx,
					   struct policy_handle *handle,
					   uint32_t *access_mask);
NTSTATUS rpccli_lsa_SetSystemAccessAccount(struct rpc_pipe_client *cli,
					   TALLOC_CTX *mem_ctx,
					   struct policy_handle *handle,
					   uint32_t access_mask);
NTSTATUS rpccli_lsa_OpenTrustedDomain(struct rpc_pipe_client *cli,
				      TALLOC_CTX *mem_ctx,
				      struct policy_handle *handle,
				      struct dom_sid2 *sid,
				      uint32_t access_mask,
				      struct policy_handle *trustdom_handle);
NTSTATUS rpccli_lsa_QueryTrustedDomainInfo(struct rpc_pipe_client *cli,
					   TALLOC_CTX *mem_ctx,
					   struct policy_handle *trustdom_handle,
					   enum lsa_TrustDomInfoEnum level,
					   union lsa_TrustedDomainInfo *info);
NTSTATUS rpccli_lsa_SetInformationTrustedDomain(struct rpc_pipe_client *cli,
						TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_OpenSecret(struct rpc_pipe_client *cli,
			       TALLOC_CTX *mem_ctx,
			       struct policy_handle *handle,
			       struct lsa_String name,
			       uint32_t access_mask,
			       struct policy_handle *sec_handle);
NTSTATUS rpccli_lsa_SetSecret(struct rpc_pipe_client *cli,
			      TALLOC_CTX *mem_ctx,
			      struct policy_handle *sec_handle,
			      struct lsa_DATA_BUF *new_val,
			      struct lsa_DATA_BUF *old_val);
NTSTATUS rpccli_lsa_QuerySecret(struct rpc_pipe_client *cli,
				TALLOC_CTX *mem_ctx,
				struct policy_handle *sec_handle,
				struct lsa_DATA_BUF_PTR *new_val,
				NTTIME *new_mtime,
				struct lsa_DATA_BUF_PTR *old_val,
				NTTIME *old_mtime);
NTSTATUS rpccli_lsa_LookupPrivValue(struct rpc_pipe_client *cli,
				    TALLOC_CTX *mem_ctx,
				    struct policy_handle *handle,
				    struct lsa_String *name,
				    struct lsa_LUID *luid);
NTSTATUS rpccli_lsa_LookupPrivName(struct rpc_pipe_client *cli,
				   TALLOC_CTX *mem_ctx,
				   struct policy_handle *handle,
				   struct lsa_LUID *luid,
				   struct lsa_StringLarge *name);
NTSTATUS rpccli_lsa_LookupPrivDisplayName(struct rpc_pipe_client *cli,
					  TALLOC_CTX *mem_ctx,
					  struct policy_handle *handle,
					  struct lsa_String *name,
					  uint16_t language_id,
					  uint16_t language_id_sys,
					  struct lsa_StringLarge **disp_name,
					  uint16_t *returned_language_id);
NTSTATUS rpccli_lsa_DeleteObject(struct rpc_pipe_client *cli,
				 TALLOC_CTX *mem_ctx,
				 struct policy_handle *handle);
NTSTATUS rpccli_lsa_EnumAccountsWithUserRight(struct rpc_pipe_client *cli,
					      TALLOC_CTX *mem_ctx,
					      struct policy_handle *handle,
					      struct lsa_String *name,
					      struct lsa_SidArray *sids);
NTSTATUS rpccli_lsa_EnumAccountRights(struct rpc_pipe_client *cli,
				      TALLOC_CTX *mem_ctx,
				      struct policy_handle *handle,
				      struct dom_sid2 *sid,
				      struct lsa_RightSet *rights);
NTSTATUS rpccli_lsa_AddAccountRights(struct rpc_pipe_client *cli,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle *handle,
				     struct dom_sid2 *sid,
				     struct lsa_RightSet *rights);
NTSTATUS rpccli_lsa_RemoveAccountRights(struct rpc_pipe_client *cli,
					TALLOC_CTX *mem_ctx,
					struct policy_handle *handle,
					struct dom_sid2 *sid,
					uint8_t remove_all,
					struct lsa_RightSet *rights);
NTSTATUS rpccli_lsa_QueryTrustedDomainInfoBySid(struct rpc_pipe_client *cli,
						TALLOC_CTX *mem_ctx,
						struct policy_handle *handle,
						struct dom_sid2 *dom_sid,
						enum lsa_TrustDomInfoEnum level,
						union lsa_TrustedDomainInfo *info);
NTSTATUS rpccli_lsa_SetTrustedDomainInfo(struct rpc_pipe_client *cli,
					 TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_DeleteTrustedDomain(struct rpc_pipe_client *cli,
					TALLOC_CTX *mem_ctx,
					struct policy_handle *handle,
					struct dom_sid2 *dom_sid);
NTSTATUS rpccli_lsa_StorePrivateData(struct rpc_pipe_client *cli,
				     TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_RetrievePrivateData(struct rpc_pipe_client *cli,
					TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_OpenPolicy2(struct rpc_pipe_client *cli,
				TALLOC_CTX *mem_ctx,
				const char *system_name,
				struct lsa_ObjectAttribute *attr,
				uint32_t access_mask,
				struct policy_handle *handle);
NTSTATUS rpccli_lsa_GetUserName(struct rpc_pipe_client *cli,
				TALLOC_CTX *mem_ctx,
				const char *system_name,
				struct lsa_String **account_name,
				struct lsa_String **authority_name);
NTSTATUS rpccli_lsa_QueryInfoPolicy2(struct rpc_pipe_client *cli,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle *handle,
				     enum lsa_PolicyInfo level,
				     union lsa_PolicyInformation **info);
NTSTATUS rpccli_lsa_SetInfoPolicy2(struct rpc_pipe_client *cli,
				   TALLOC_CTX *mem_ctx,
				   struct policy_handle *handle,
				   enum lsa_PolicyInfo level,
				   union lsa_PolicyInformation *info);
NTSTATUS rpccli_lsa_QueryTrustedDomainInfoByName(struct rpc_pipe_client *cli,
						 TALLOC_CTX *mem_ctx,
						 struct policy_handle *handle,
						 struct lsa_String *trusted_domain,
						 enum lsa_TrustDomInfoEnum level,
						 union lsa_TrustedDomainInfo *info);
NTSTATUS rpccli_lsa_SetTrustedDomainInfoByName(struct rpc_pipe_client *cli,
					       TALLOC_CTX *mem_ctx,
					       struct policy_handle *handle,
					       struct lsa_String trusted_domain,
					       enum lsa_TrustDomInfoEnum level,
					       union lsa_TrustedDomainInfo *info);
NTSTATUS rpccli_lsa_EnumTrustedDomainsEx(struct rpc_pipe_client *cli,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle *handle,
					 uint32_t *resume_handle,
					 struct lsa_DomainListEx *domains,
					 uint32_t max_size);
NTSTATUS rpccli_lsa_CreateTrustedDomainEx(struct rpc_pipe_client *cli,
					  TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_CloseTrustedDomainEx(struct rpc_pipe_client *cli,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle *handle);
NTSTATUS rpccli_lsa_QueryDomainInformationPolicy(struct rpc_pipe_client *cli,
						 TALLOC_CTX *mem_ctx,
						 struct policy_handle *handle,
						 uint16_t level,
						 union lsa_DomainInformationPolicy *info);
NTSTATUS rpccli_lsa_SetDomainInformationPolicy(struct rpc_pipe_client *cli,
					       TALLOC_CTX *mem_ctx,
					       struct policy_handle *handle,
					       uint16_t level,
					       union lsa_DomainInformationPolicy *info);
NTSTATUS rpccli_lsa_OpenTrustedDomainByName(struct rpc_pipe_client *cli,
					    TALLOC_CTX *mem_ctx,
					    struct policy_handle *handle,
					    struct lsa_String name,
					    uint32_t access_mask,
					    struct policy_handle *trustdom_handle);
NTSTATUS rpccli_lsa_TestCall(struct rpc_pipe_client *cli,
			     TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_LookupSids2(struct rpc_pipe_client *cli,
				TALLOC_CTX *mem_ctx,
				struct policy_handle *handle,
				struct lsa_SidArray *sids,
				struct lsa_RefDomainList **domains,
				struct lsa_TransNameArray2 *names,
				uint16_t level,
				uint32_t *count,
				uint32_t unknown1,
				uint32_t unknown2);
NTSTATUS rpccli_lsa_LookupNames2(struct rpc_pipe_client *cli,
				 TALLOC_CTX *mem_ctx,
				 struct policy_handle *handle,
				 uint32_t num_names,
				 struct lsa_String *names,
				 struct lsa_RefDomainList **domains,
				 struct lsa_TransSidArray2 *sids,
				 enum lsa_LookupNamesLevel level,
				 uint32_t *count,
				 uint32_t unknown1,
				 uint32_t unknown2);
NTSTATUS rpccli_lsa_CreateTrustedDomainEx2(struct rpc_pipe_client *cli,
					   TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_CREDRWRITE(struct rpc_pipe_client *cli,
			       TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_CREDRREAD(struct rpc_pipe_client *cli,
			      TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_CREDRENUMERATE(struct rpc_pipe_client *cli,
				   TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_CREDRWRITEDOMAINCREDENTIALS(struct rpc_pipe_client *cli,
						TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_CREDRREADDOMAINCREDENTIALS(struct rpc_pipe_client *cli,
					       TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_CREDRDELETE(struct rpc_pipe_client *cli,
				TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_CREDRGETTARGETINFO(struct rpc_pipe_client *cli,
				       TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_CREDRPROFILELOADED(struct rpc_pipe_client *cli,
				       TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_LookupNames3(struct rpc_pipe_client *cli,
				 TALLOC_CTX *mem_ctx,
				 struct policy_handle *handle,
				 uint32_t num_names,
				 struct lsa_String *names,
				 struct lsa_RefDomainList **domains,
				 struct lsa_TransSidArray3 *sids,
				 enum lsa_LookupNamesLevel level,
				 uint32_t *count,
				 uint32_t unknown1,
				 uint32_t unknown2);
NTSTATUS rpccli_lsa_CREDRGETSESSIONTYPES(struct rpc_pipe_client *cli,
					 TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_LSARREGISTERAUDITEVENT(struct rpc_pipe_client *cli,
					   TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_LSARGENAUDITEVENT(struct rpc_pipe_client *cli,
				      TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_LSARUNREGISTERAUDITEVENT(struct rpc_pipe_client *cli,
					     TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_lsaRQueryForestTrustInformation(struct rpc_pipe_client *cli,
						    TALLOC_CTX *mem_ctx,
						    struct policy_handle *handle,
						    struct lsa_String *trusted_domain_name,
						    uint16_t unknown,
						    struct lsa_ForestTrustInformation **forest_trust_info);
NTSTATUS rpccli_lsa_LSARSETFORESTTRUSTINFORMATION(struct rpc_pipe_client *cli,
						  TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_CREDRRENAME(struct rpc_pipe_client *cli,
				TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_LookupSids3(struct rpc_pipe_client *cli,
				TALLOC_CTX *mem_ctx,
				struct lsa_SidArray *sids,
				struct lsa_RefDomainList **domains,
				struct lsa_TransNameArray2 *names,
				uint16_t level,
				uint32_t *count,
				uint32_t unknown1,
				uint32_t unknown2);
NTSTATUS rpccli_lsa_LookupNames4(struct rpc_pipe_client *cli,
				 TALLOC_CTX *mem_ctx,
				 uint32_t num_names,
				 struct lsa_String *names,
				 struct lsa_RefDomainList **domains,
				 struct lsa_TransSidArray3 *sids,
				 enum lsa_LookupNamesLevel level,
				 uint32_t *count,
				 uint32_t unknown1,
				 uint32_t unknown2);
NTSTATUS rpccli_lsa_LSAROPENPOLICYSCE(struct rpc_pipe_client *cli,
				      TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_LSARADTREGISTERSECURITYEVENTSOURCE(struct rpc_pipe_client *cli,
						       TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_LSARADTUNREGISTERSECURITYEVENTSOURCE(struct rpc_pipe_client *cli,
							 TALLOC_CTX *mem_ctx);
NTSTATUS rpccli_lsa_LSARADTREPORTSECURITYEVENT(struct rpc_pipe_client *cli,
					       TALLOC_CTX *mem_ctx);
#endif /* __CLI_LSARPC__ */
